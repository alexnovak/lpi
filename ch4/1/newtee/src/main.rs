#![feature(start)]
#![no_std]

use core::ffi::c_void;
use core::panic::PanicInfo;

#[link(name = "c")]
extern "C" {
    fn write(fd: i32, buf: *mut c_void, count: u32);
    fn read(fd: i32, buf: *mut c_void, count: u32) -> i32;
    fn open(pathname: *const char, flags: i32, mode: i32) -> i32;
}

// FDs for the classics.
const STDIN: i32 = 0;
const STDOUT: i32 = 1;
const STDERR: i32 = 2;

// Open options
const O_WRONLY: i32 = 01;
const O_CREAT: i32 = 0100;
const O_TRUNC: i32 = 01000;

fn err_exit(message: &str) -> isize {
    let len = message.len();
    let buf: *mut c_void = message.as_ptr() as *mut c_void;
    unsafe { write(STDERR, buf, len as u32) };
    return 1;
}

#[start]
fn main(argc: isize, argv: *const *const u8) -> isize {
    if argc < 2 {
        return err_exit("Wrong number of arguments!");
    }
    let target_options = O_WRONLY | O_CREAT | O_TRUNC;
    unsafe {
        let fd = open((argv as *const [char])[1], target_options, 0644);
        if fd < 0 {
            return err_exit("Cannot open file!");
        }
    }
    0
}

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}
