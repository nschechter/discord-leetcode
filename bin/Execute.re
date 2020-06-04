let run = (interpreter, input, code) => {
  switch (interpreter) {
  | "js" =>
    Node.execute(code ++ "\n" ++ "console.log(solve(" ++ input ++ "))")
  | "python" =>
    Python.execute(code ++ "\n" ++ "print(solve(" ++ input ++ "))")
  | _ => ("", "Unknown interpereter")
  };
};