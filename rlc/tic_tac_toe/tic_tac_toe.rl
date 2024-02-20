# This file contains an implementation of a 3x3 tic tac toe. The game crashes when a player wins by
#	filling the second diagonal. We are expecting the fuzzer to find this error.
import serialization.to_byte_vector
import fuzzer.cpp_functions
import fuzzer.utils

fun crash(Int x) {x != 4}:
	return

ent Board:
	Int[9] slots
	Bool playerTurn


	fun get(Int x, Int y) -> Int:
		return self.slots[x + (y*3)]

	fun set(Int x, Int y, Int val): 
		self.slots[x + (y * 3)] = val

	fun full() -> Bool:
		let x = 0

		while x < 3:
			let y = 0
			while y < 3:
				if self.get(x, y) == 0:
					return false
				y = y + 1
			x = x + 1

		return true

	fun three_in_a_line_player_row(Int player_id, Int row) -> Bool:
		return self.get(0, row) == self.get(1, row) and self.get(0, row) == self.get(2, row) and self.get(0, row) == player_id

	fun three_in_a_line_player(Int player_id) -> Bool:
		let x = 0
		while x < 3:
			if self.get(x, 0) == self.get(x, 1) and self.get(x, 0) == self.get(x, 2) and self.get(x, 0) == player_id:
				return true

			if self.three_in_a_line_player_row(player_id, x):
				return true
			x = x + 1

		if self.get(0, 0) == self.get(1, 1) and self.get(0, 0) == self.get(2, 2) and self.get(0, 0) == player_id:
			return true

		if self.get(0, 2) == self.get(1, 1) and self.get(0, 2) == self.get(2, 0) and self.get(0, 2) == player_id:
			crash(4)
			return true

		return false

	fun current_player() -> Int:
		return int(self.playerTurn) + 1

	fun next_turn():
		self.playerTurn = !self.playerTurn

act play() -> TicTacToe:
	frm board : Board
	frm score = 10
	while !board.full():
		act mark(Int x, Int y) {
			x < 3,
			x >= 0,
			y < 3,
			y >= 0,
			board.get(x, y) == 0
		}
		score = score - 1

		board.set(x, y, board.current_player())

		if board.three_in_a_line_player(board.current_player()):
			return

		board.next_turn()

fun gen_printer_parser():
	let state = play()
	let serialized = as_byte_vector(state)
	from_byte_vector(state, serialized)