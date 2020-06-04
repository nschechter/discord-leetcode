open Yojson.Basic.Util;

type input = {
  input: string,
  value: bool,
};

type challenge = {
  id: int,
  name: string,
  description: string,
  inputs: list(input),
};

let loadFromJson = () => {
  Yojson.Basic.from_file("./bin/challenges.json")
  |> member("challenges")
  |> to_list
  |> List.map(json =>
       {
         id: json |> member("id") |> to_int,
         name: json |> member("name") |> to_string,
         description: json |> member("description") |> to_string,
         inputs:
           json
           |> member("inputs")
           |> to_list
           |> List.map(json =>
                {
                  input: json |> member("input") |> to_string,
                  value: json |> member("value") |> to_bool,
                }
              ),
       }
     );
};

let getChallengeFromId = (id, challenges) => {
  challenges |> List.find(c => c.id == id);
};

let messageChallenge = (message, challenge) => {
  ReDiscord.Message.reply(
    ":exclamation: "
    ++ challenge.name
    ++ " :exclamation:\n\n"
    ++ "Description:\n"
    ++ "```"
    ++ challenge.description
    ++ "```"
    ++ "\n!leetcode solve ```js\nfunction solve(input) {\n  ...\n  return true\n}```\n!leetcode inputs",
    message,
  )
  |> ignore;

  challenge;
};

let messageInputs = (message, challenge) => {
  let content =
    challenge.inputs
    |> List.map(i =>
         "`"
         ++ i.input
         ++ "`"
         ++ " => "
         ++ "`"
         ++ (i.value |> string_of_bool)
         ++ "`"
       )
    |> Base.List.fold_right(
         ~f=(acc, final) => final ++ acc ++ "\n",
         ~init=challenge.name ++ " inputs:\n\n",
       );

  ReDiscord.Message.reply(content, message);
};