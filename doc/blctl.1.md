blctl
=====

## NAME

blctl - control device backlight through linux /sys interface

## SYNOPSIS

`blctl DEVICE [ up | down | get | set VAL ]`

## DESCRIPTION

Modify backlight intensity of `DEVICE`. `DEVICE` is in the form `CLASS/NAME`. It
is possible to list controllable devices by listing the contents of
`/sys/class/backlight` and `/sys/class/leds` for instance.

### up

Increase backlight intensity of `DEVICE` by 5%.

### down

Decrease backlight intensity of `DEVICE` by 5%.

### get

Display backlight intensity of `DEVICE`.

### set

Set backlight intensity of `DEVICE` to `VAL`.

## EXAMPLES

* `blctl backlight/acpi_video0 get`
* `blctl backlight/acpi_video0 set 50`
* `blctl leds/smc::kbd_backlight up`

## AUTHORS

Written by Stephane Sezer `<sas@cd80.net>`.

## COPYRIGHT

This tool is published under the BSD 3-clause license.
