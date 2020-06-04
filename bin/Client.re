open ReDiscord;
open Challenge;

let _TOKEN = Sys.getenv_opt("DISCORD_BOT_TOKEN");

let getArgs = msg => Base.String.split(~on=' ', msg);

let currentChallengeId = ref(1);

let getCode = input => {
  let reg = Re.Pcre.regexp({|(```)(js|python)([\s\S]*)(```)|});

  try({
    let groups = Re.Pcre.exec(~rex=reg, input);
    Some((
      Re.Pcre.get_substring(groups, 2),
      Re.Pcre.get_substring(groups, 3),
    ));
  }) {
  | Not_found => None
  };
};

let rec solve = (interpreter, code, inputs) => {
  switch (inputs) {
  | [] => (true, "")
  | [head, ...tail] =>
    let output = Execute.run(interpreter, head.input, code);
    let returnValue =
      fst(output)
      |> Base.String.split(~on='\n')
      |> List.rev
      |> List.hd
      |> String.trim;

    returnValue
    |> bool_of_string_opt
    |> (
      fun
      | Some(val_) => {
          val_ == head.value
            ? solve(interpreter, code, tail)
            : (false, fst(output) ++ "\n\nFailed on input: " ++ head.input);
        }
      | None => (false, fst(output) ++ snd(output))
    );
  };
};

let getRandomChallenge = challenges => {
  let challengeId = (challenges |> List.length |> Random.int) + 1;
  challenges |> getChallengeFromId(challengeId);
};

let onMessage = (message: Message.t) =>
  if (Base.String.is_prefix(~prefix="!leetcode", message.content)) {
    let args = getArgs(message.content);

    switch (List.nth_opt(args, 1)) {
    | Some("new") =>
      Challenge.loadFromJson()
      |> getRandomChallenge
      |> messageChallenge(message)
      |> (challenge => currentChallengeId := challenge.id)
    | Some("inputs") =>
      Challenge.loadFromJson()
      |> List.find(c => c.id == currentChallengeId^)
      |> messageInputs(message)
      |> ignore
    | Some("solve") =>
      let challenge =
        Challenge.loadFromJson() |> getChallengeFromId(currentChallengeId^);

      let output =
        message.content
        |> getCode
        |> (
          fun
          | Some((interpreter, code)) => {
              let val_ = solve(interpreter, code, challenge.inputs);
              fst(val_) ? ("Success!", "") : ("", snd(val_));
            }
          | None => ("", "Unable to parse code")
        );

      Message.reply(fst(output), message) |> ignore;
      Message.reply(snd(output), message) |> ignore;
    | _ =>
      List.map(
        a => {
          print_endline(a);
          a;
        },
        args,
      )
      |> ignore
    };
  };

let onReady = () => {
  print_endline("Connected!");
};

let start = () => {
  switch (_TOKEN) {
  | Some(token) =>
    Discord.make(~debug=false, ~onReady, ~onMessage, token) |> Lwt_main.run
  | None =>
    print_endline("ERROR: No token found, try exporting DISCORD_BOT_TOKEN")
  };
};