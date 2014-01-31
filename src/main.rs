extern mod glob;
use glob::glob;
use std::io::File;
use std::io::Open;
use std::io::Read;
use std::io::Write;
use std::os;
use std::libc;
use std::io::fs;

fn exit(exit_code: i32) {
  unsafe {
    libc::exit(exit_code);
  }
}

fn print_usage () {
  println!("usage: {} [ list | DEVICE [ up | down | get | set VAL ]]\n", os::args()[0]);
}

fn get(device: &str) -> ~str {
  let p = Path::new("/sys/class/" + device + "/brightness");
  return File::open_mode(&p, Open, Read).read_to_str()
}

fn get_max(device: &str) -> ~str {
  let p = Path::new("/sys/class/" + device + "/max_brightness");
  return File::open_mode(&p, Open, Read).read_to_str()
}

fn set(device: &str, level: &str) {
  let p = Path::new("/sys/class/" + device + "/brightness");
  File::open_mode(&p, Open, Write).write_str(level);
}

fn list_glob(pattern: &str) {
  for path in glob(pattern) {
    let relative_from = &Path::new("/sys/class");
    match path.path_relative_from(relative_from) {
      Some(path) => print!("{}\n", path.display()),
      None => {}
    }
  }
}

fn list () {
  println!("List of compatible devices:");
  list_glob("/sys/class/backlight/*");
  list_glob("/sys/class/leds/*");
}

fn validate_device (device: &str) {
  let p = Path::new("/sys/class/" + device);
  match std::io::result(|| std::io::fs::stat(&p)) {
    Ok(s) => {},
    Err(e) => {
      println!("invalid device!");
      print_usage();
      exit(1);
    }
  }
}


fn change (device: &str, percent: int) {
  let cur_level = from_str::<int>(get(device)).unwrap_or(0);
  let max_level = from_str::<int>(get_max(device)).unwrap_or(1);
  let mut cur_perc = cur_level * 100 / max_level;
  cur_perc += percent;
  if cur_perc < 0 {
    cur_perc = 0;
  }
  if cur_perc > 100 {
    cur_perc = 100;
  }
  let new_level = cur_perc * max_level / 100;
  set(device, new_level.to_str());
}

fn down (device: &str) {
  change(device, -10);
}

fn up (device: &str) {
  change(device, 10);
}

fn main () {
  let args = os::args();
  print!("Length of args: {}\n", args.len());

  if (args.len() == 2) {
    if (args[1] == ~"list") {
      list();
      exit(0);
    } else {
      print_usage();
    }
  }

  validate_device(args[1]);

  if (args.len() == 3) {
    if (args[2] == ~"get") {
      print!("{}", get(args[1]));
      exit(0);
    } else if (args[2] == ~"up") {
      up(args[1]);
      exit(0);
    } else if (args[2] == ~"down") {
      down(args[1]);
      exit(0);
    }
  }

  if (args.len() == 4 && args[2] == ~"set") {
      set(args[1], args[3]);
      exit(0);
  }

  print_usage();
  exit(1);
}
