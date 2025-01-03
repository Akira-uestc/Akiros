use std::str::SplitWhitespace;

#[derive(Debug)]
pub struct ParsedCommand {
    pub program: String,
    pub args: Vec<String>,
    pub output_redirection: Option<String>,
    pub input_redirection: Option<String>,
    pub pipeline: Option<Box<ParsedCommand>>,
}

pub fn parse_command(command: &str) -> Result<ParsedCommand, String> {
    let mut tokens = command.split_whitespace();
    parse_tokens(&mut tokens)
}

fn parse_tokens(tokens: &mut SplitWhitespace) -> Result<ParsedCommand, String> {
    let mut program = String::new();
    let mut args = Vec::new();
    let mut output_redirection = None;
    let mut input_redirection = None;
    let mut pipeline = None;

    while let Some(token) = tokens.next() {
        match token {
            "|" => {
                let next_command = parse_tokens(tokens)?;
                pipeline = Some(Box::new(next_command));
                break;
            }
            ">" => {
                if let Some(file) = tokens.next() {
                    output_redirection = Some(file.to_string());
                } else {
                    return Err("Missing file after >".to_string());
                }
            }
            "<" => {
                if let Some(file) = tokens.next() {
                    input_redirection = Some(file.to_string());
                } else {
                    return Err("Missing file after <".to_string());
                }
            }
            _ => {
                if program.is_empty() {
                    program = token.to_string();
                } else {
                    args.push(token.to_string());
                }
            }
        }
    }

    if program.is_empty() {
        return Err("No program specified".to_string());
    }

    Ok(ParsedCommand {
        program,
        args,
        output_redirection,
        input_redirection,
        pipeline,
    })
}
