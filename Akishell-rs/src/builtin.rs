use std::env;

pub fn handle_builtin(command: &str, if_loop: &mut bool) -> bool {
    let mut parts = command.split_whitespace();
    if let Some(cmd) = parts.next() {
        match cmd {
            "exit" => {
                builtin_exit(if_loop);
                true
            }
            "cd" => {
                let path = parts.next().unwrap_or("~");
                builtin_cd(path);
                true
            }
            "exec" => {
                builtin_exec();
                true
            }
            "help" => {
                builtin_help();
                true
            }
            _ => false,
        }
    } else {
        false
    }
}

fn builtin_exit(if_loop: &mut bool) {
    println!("Exiting...");
    *if_loop = false;
}

fn builtin_cd(path: &str) {
    let target_path = if path == "~" {
        env::var("HOME").unwrap_or_else(|_| "/".to_string())
    } else {
        path.to_string()
    };

    if let Err(e) = env::set_current_dir(&target_path) {
        eprintln!("cd: {}: {}", target_path, e);
    } else {
        println!("Changing directory to {}", target_path);
    }
}

fn builtin_exec() {
    println!("Execute command not implemented.");
}

fn builtin_help() {
    println!("Available commands: exit, cd, exec, help.");
}
