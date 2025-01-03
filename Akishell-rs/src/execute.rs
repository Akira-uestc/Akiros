use crate::parse::ParsedCommand;
use std::process::{Command, Stdio};

pub fn execute_command(parsed_command: &ParsedCommand) -> Result<(), String> {
    fn execute_pipeline(
        parsed_command: &ParsedCommand,
        input: Option<Stdio>,
    ) -> Result<(), String> {
        let mut command = Command::new(&parsed_command.program);
        command.args(&parsed_command.args);

        if let Some(ref input_redirection) = parsed_command.input_redirection {
            command.stdin(Stdio::from(
                std::fs::File::open(input_redirection).map_err(|e| e.to_string())?,
            ));
        } else if let Some(stdin) = input {
            command.stdin(stdin);
        }

        let output = if let Some(ref output_redirection) = parsed_command.output_redirection {
            Stdio::from(std::fs::File::create(output_redirection).map_err(|e| e.to_string())?)
        } else if parsed_command.pipeline.is_some() {
            Stdio::piped()
        } else {
            Stdio::inherit()
        };

        command.stdout(output);

        let mut child = command.spawn().map_err(|e| e.to_string())?;

        if let Some(ref pipeline) = parsed_command.pipeline {
            if let Some(stdout) = child.stdout.take() {
                execute_pipeline(pipeline, Some(Stdio::from(stdout)))?;
            }
        }

        child.wait().map_err(|e| e.to_string()).map(|_| ())
    }

    execute_pipeline(parsed_command, None)
}
