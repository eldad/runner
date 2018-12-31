/*
   App View
 */

let debug = false;

Random.self_init();

let generate_sequence = () => Array.make(2, 0) |> Array.map(_v => Random.int(4));

/* Use polymorphic variants, because they get converted to strings */
[@bs.deriving jsConverter]
type stage = [ | `Idle | `Play | `Input];

type action =
  | ColorHighlight(int)
  | ColorDeselect
  | Play
  | WaitOnPlayerInput
  | PlayerInput(array(int));

type state = {
  color_highlighted: option(int),
  stage,
  sequence: array(int),
  points: int,
};

let initialState = () => {color_highlighted: None, stage: `Idle, sequence: [||], points: 0};

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
    | PlayerInput(sequence) =>
      let points = state.points + (sequence == state.sequence ? 1 : 0);
      ReasonReact.Update({...state, points, stage: `Idle});
    | _ => ReasonReact.NoUpdate
    },
  didUpdate: (oldAndNewSelf) => {
    let self = oldAndNewSelf.newSelf;
    let context = Dom_html.getCanvas2DContext("canvas");
    Dom_html.requestAnimationFrame(_f => CanvasRender.render(~context, ~width=320, ~height=640, ~points=self.state.points, ())) |> ignore;
  },
  render: self => {
    let debug_info =
      debug ?
        <div className=Glamor.(css([backgroundColor("#000")]))>
          {self.state.stage |> stageToJs |> ReasonReact.string}
          {self.state.sequence |> Array.map(n => ReasonReact.string(n |> string_of_int)) |> ReasonReact.array}
        </div> :
        ReasonReact.null;

    <div className=Glamor.(css([display("flex"), alignItems("center"), flexDirection("column")]))>
      <div className=Glamor.(css([backgroundColor("#333")]))>
        {Printf.sprintf("Points: %d", self.state.points) |> ReasonReact.string}
      </div>
      debug_info
      {
        switch (self.state.stage) {
        | `Idle =>
          <div>
            <button onClick=(_e => self.send(Play))> {"Play" |> ReasonReact.string} </button>
            <ColorBlocks highlight=?{self.state.color_highlighted} onBlockClick=(i => self.send(ColorHighlight(i))) />
          </div>
        | `Play => <ColorBlocksAnimation sequence={self.state.sequence} onFinish=(() => self.send(WaitOnPlayerInput)) />
        | `Input =>
          <ColorBlocksInput
            onFinish=(sequence => self.send(PlayerInput(sequence)))
            sequence_length={self.state.sequence |> Array.length}
          />
        }
      }
    </div>;
  },
};
