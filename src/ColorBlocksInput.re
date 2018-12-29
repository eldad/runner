/*
     Color Blocks (User) Input
 */

let debug = false;

type action =
  | Input(int);

type state = {sequence: array(int)};

let is_sequence_full = (length, state) => Array.length(state.sequence) == length;

let initialState = () => {sequence: [||]};

let component = ReasonReact.reducerComponent("ColorBlocksInput");

let make = (~onFinish, ~sequence_length, _children) => {
  ...component,
  initialState,
  reducer: (action, state) =>
    switch (action) {
    | Input(i) =>
      is_sequence_full(sequence_length, state) ?
        ReasonReact.NoUpdate :
        {
          state.sequence |> Js.Array.push(i) |> ignore;
          is_sequence_full(sequence_length, state) ?
            ReasonReact.UpdateWithSideEffects(state, (_self => onFinish(state.sequence))) : ReasonReact.Update(state);
        }
    },
  render: self =>
    <div>
      {
        debug ?
          <div> {self.state.sequence |> Array.map(n => ReasonReact.string(n |> string_of_int)) |> ReasonReact.array} </div> :
          ReasonReact.null
      }
      <ColorBlocks onBlockClick={i => self.send(Input(i))} disabled={self.state |> is_sequence_full(sequence_length)} />
    </div>,
};
