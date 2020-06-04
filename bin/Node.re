open Core;

let execute = code => {
  let inputChannels =
    Unix.open_process_full("node -e '" ++ code ++ "'", ~env=[||]);
  let stdOut =
    In_channel.fold_lines(
      ~f=(acc, sum) => acc ++ "\n" ++ sum,
      ~init="",
      inputChannels.stdout,
    );
  let stdErr =
    In_channel.fold_lines(
      ~f=(acc, sum) => acc ++ "\n" ++ sum,
      ~init="",
      inputChannels.stderr,
    );
  Unix.close_process_full(inputChannels) |> ignore;
  (stdOut, stdErr);
};