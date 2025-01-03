use std::fs::File;
use std::io::{self, BufRead};

pub fn execute_from_file(filepath: &str, if_loop: &mut bool) {
    while *if_loop {
        if let Ok(file) = File::open(filepath) {
            let reader = io::BufReader::new(file);
            for line in reader.lines() {
                match line {
                    Ok(command_line) => {
                        if command_line.trim().is_empty() || command_line.trim().starts_with('#') {
                            continue;
                        }
                        if crate::builtin::handle_builtin(&command_line, if_loop) {
                            continue;
                        }
                        match crate::parse::parse_command(&command_line) {
                            Ok(parsed_command) => {
                                if let Err(e) = crate::execute::execute_command(&parsed_command) {
                                    eprintln!("Error executing command '{}': {}", command_line, e);
                                }
                            }
                            Err(e) => eprintln!("Error parsing command '{}': {}", command_line, e),
                        }
                    }
                    Err(e) => eprintln!("Error reading line: {}", e),
                }
            }
            *if_loop = false;
        } else {
            eprintln!("Error opening file: {}", filepath);
            break;
        }
    }
}
