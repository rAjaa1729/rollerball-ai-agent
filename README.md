# Rollerball AI Agent

An AI bot for "Rollerball", a chess-variant game, playing via minimax
search with alpha-beta pruning and a material/positional evaluation
function. Built on a provided game-engine scaffold (board representation,
move generation, a websocket server for a UI to talk to the engine).

## How it works

- `src/engine.cpp` / `engine.hpp` — the bot itself: `MaxVal`/`MinVal`
  minimax with alpha-beta pruning, evaluating positions using piece
  values (king/rook/bishop weights).
- `src/board.cpp`, `server.cpp`, `uciws.cpp`, `rollerball.cpp` — the
  provided board representation and a UCI-style websocket server that
  drives a game between the bot and a UI.
- `src/bindings.cpp`, `engine_py.cpp`, `setup.py` — optional pybind11
  bindings to drive the engine from Python instead of the C++ server.
- `web/` — the provided browser UI (vanilla HTML/JS + chess piece assets)
  to watch/play games against the bot.
- `scripts/` — helper scripts to launch the engine server and UI together.

## Run it

```bash
make rollerball                 # builds bin/rollerball
./scripts/run_servers.sh        # start engine + UI servers
./scripts/run_ui.sh             # open the web UI in your browser
```

Building needs the [asio](https://think-async.com/Asio/) (standalone,
header-only) and [websocketpp](https://github.com/zaphoyd/websocketpp)
libraries under `include/` — not vendored in this repo to keep it small.
Download them and drop `asio/`, `asio.hpp`, `websocketpp/` (and
`popl.hpp` if you use the CLI arg parser) into `include/` before building.

## `archive/`

Alternate engine implementations explored during development, none of
which made it into the final version: `raja-engine-variant.cpp`,
`suryanshu-engine-variant.cpp`, and two early standalone prototypes
(`engine_i_o.cpp`, `preengine.cpp`) written before the client/server
architecture above existed.

## Notes

`docs/assignment-brief.pdf` is the original assignment spec this project
was built for; `docs/original-submission-readme.md` is the team's
original submission readme (collaborator credits).
