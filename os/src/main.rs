#![no_std]
#![no_main]

#[macro_use]
mod console;
mod lang_item;
mod sbi;

use core::arch::global_asm;

use sbi::shutdown;

global_asm!(include_str!("entry.asm"));

#[no_mangle]
pub fn rust_main() -> ! {
	clear_bss();
	println!("Hello World!");
	shutdown(false)
}

fn clear_bss() {
    extern "C" {
        fn sbss();
        fn ebss();
    }
    (sbss as usize..ebss as usize).for_each(|a| {
        unsafe { (a as *mut u8).write_volatile(0) }
    });
}
