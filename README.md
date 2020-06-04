# Discord-Leetcode

Solve leetcode-esque questions, discuss possible solutions, debate trade offs, all in a social place!

## Usage

You need Esy, you can install the beta using [npm](https://npmjs.com):

    % npm install -g esy@latest

> NOTE: Make sure `esy --version` returns at least `0.5.4` for this project to build.

Then run the `esy` command from this project root to install and build dependencies.

    % esy

Now you can run your editor within the environment (which also includes merlin):

    % esy $EDITOR
    % esy vim

After you make some changes to source code, you can re-run project's build
again with the same simple `esy` command.

    % esy

Shell into environment:

    % esy shell

## Running Discord-Leetcode locally:

    % echo DISCORD_BOT_TOKEN = token
    % esy x Main


## Create Prebuilt Release:

`esy` allows creating prebuilt binary packages for your current platform, with
no dependencies.

    % esy npm-release
    % cd _release
    % npm publish