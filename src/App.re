/*
   App View
 */

Random.self_init();

let generate_sequence = () => Array.make(6, 0) |> Array.map(_v => Random.int(4));

/* Use polymorphic variants, because they get converted to strings */
[@bs.deriving jsConverter]
type stage = [ | `Idle | `Play | `Input];

type action =
  | ColorHighlight(int)
  | ColorDeselect
  | Play
  | WaitOnPlayerInput;

type state = {
  color_highlighted: option(int),
  stage,
  sequence: array(int),
};

let initialState = () => {color_highlighted: None, stage: `Idle, sequence: [||]};

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  initialState,
  reducer: (action, state) =>
    switch (action) {
    | ColorHighlight(i) => ReasonReact.Update({...state, color_highlighted: Some(i)})
    | ColorDeselect => ReasonReact.Update({...state, color_highlighted: None})
    | Play when state.stage == `Idle => ReasonReact.Update({...state, sequence: generate_sequence(), stage: `Play})
    | WaitOnPlayerInput => ReasonReact.Update({...state, stage: `Input})
    | _ => ReasonReact.NoUpdate
    },
  render: self =>
    <div className=Glamor.(css([display("flex"), alignItems("center"), flexDirection("column")]))>
      <div className=Glamor.(css([backgroundColor("#000")]))> {self.state.stage |> stageToJs |> ReasonReact.string}
      {self.state.sequence |> Array.map((n) => ReasonReact.string(n |> string_of_int)) |> ReasonReact.array}
      </div>
      {
        switch (self.state.stage) {
        | `Idle =>
          <div>
            <button onClick=(_e => self.send(Play))> {"Play" |> ReasonReact.string} </button>
            <ColorBlocks highlight=?{self.state.color_highlighted} onBlockClick=(i => self.send(ColorHighlight(i))) />
          </div>
        | `Play => <ColorBlocksAnimation sequence={self.state.sequence} onFinish=(_e => self.send(WaitOnPlayerInput)) />
        | `Input => ReasonReact.null
        }
      }
    </div>,
};
