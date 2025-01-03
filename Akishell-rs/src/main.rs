mod builtin;
mod execute;
mod file;
mod parse;

use std::fs;
use std::io::{self, Write};

use file::execute_from_file;

fn main() {
    let args: Vec<String> = std::env::args().collect();
    let mut if_loop: bool = true;

    if args.len() >= 2 {
        match fs::metadata(&args[1]) {
            Ok(metadata) => {
                if metadata.is_file() {
                    execute_from_file(&args[1], &mut if_loop);
                } else {
                    eprintln!("WARN: Not a valid file.");
                }
            }
            Err(err) => {
                eprintln!("ERROR: Could not access file metadata: {}", err);
            }
        }
    }

    while if_loop {
        print!("-> ");
        io::stdout().flush().unwrap();

        let mut input = String::new();
        if io::stdin().read_line(&mut input).is_err() {
            eprintln!("ERROR: Failed to read input.");
            continue;
        }

        let command = input.trim();

        if builtin::handle_builtin(command, &mut if_loop) {
            continue;
        }
        match parse::parse_command(command) {
            Ok(parsed_command) => {
                if let Err(err) = execute::execute_command(&parsed_command) {
                    eprintln!("ERROR: Failed to execute command: {}", err);
                }
            }
            Err(err) => {
                eprintln!("ERROR: Failed to parse command: {}", err);
            }
        }
    }
}
