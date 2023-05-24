from collections import defaultdict
from queue import Queue

class NFA():
 
	characters = set()
	set_of_states = set()
	accepting_states = set()
	current_state = 0
	state_num = 0
	transition_function = defaultdict(set)
	RE = "" # regular expression
 
	# constructor
	def __init__(self, RE):
		self.RE = RE
	
	# build the alphabet from the characters
	def build_alphabet(self):
		for ch in range(0, len(self.RE)):
			if self.RE[ch] != '+' and self.RE[ch] != '*' and self.RE[ch] != '(' and self.RE[ch] != ')' and self.RE[ch] != ' ':
				self.characters.add(self.RE[ch])
		self.characters.add('E') # for the epsilon transitions
	
	# utility function for the recursion while building the NFA
	def generate_NFA(self, sub_initial_state, i):
		close_state = 0
		while i < len(self.RE):
			if self.RE[i] in self.characters and i < len(self.RE):
				if self.RE[i+1] != '*':
					self.transition_function[(self.current_state, self.RE[i])].add(self.state_num+1)
					self.current_state = self.state_num + 1
					self.state_num = self.state_num + 1
					self.set_of_states.add(self.current_state)
				else:
					self.transition_function[(self.current_state, self.RE[i])].add(self.state_num)
					i = i + 1
	 
			elif self.RE[i] in self.characters:
				if close_state != 0:
					self.transition_function[(self.current_state, self.RE[i])].add(self.state_num+1)
					self.current_state = self.state_num + 1
					self.state_num = self.state_num + 1
					self.set_of_states.add(self.current_state)
	 
			elif self.RE[i] == '+':
				if close_state == 0:
					self.current_state = self.state_num + 1
					self.state_num = self.state_num + 1
					self.set_of_states.add(self.current_state)
					close_state = self.current_state
					self.current_state = sub_initial_state
					self.transition_function[(self.state_num-1, 'E')].add(close_state)
				else:
					self.transition_function[(self.state_num, 'E')].add(close_state)
					self.current_state = sub_initial_state
	 
			elif self.RE[i] == '(':
				_sub_initial_state = self.current_state
				i = self.generate_NFA(_sub_initial_state, i+1)

			else:
				# self.RE[i] == ')'
				if i < len(self.RE)-1 and self.RE[i+1] == '*':
					if close_state == 0:
						self.transition_function[(self.state_num, 'E')].add(sub_initial_state)
					else:
						self.transition_function[(self.current_state, 'E')].add(close_state)
						self.transition_function[(close_state, 'E')].add(sub_initial_state)
					i = i + 1
					self.current_state = sub_initial_state
				else:
					if close_state != 0:
						self.transition_function[(self.current_state, 'E')].add(close_state)
						self.current_state = close_state
				return i
			i = i + 1
 
	def build_NFA(self):
		self.build_alphabet()
		self.set_of_states.add(0)
		initial_state = 0
		close_state = 0
  
		i = 0
		while i < len(self.RE):
			if self.RE[i] in self.characters and i < len(self.RE):
				if self.RE[i+1] != '*':
					self.transition_function[(self.current_state, self.RE[i])].add(self.state_num+1)
					self.current_state = self.state_num + 1
					self.state_num = self.state_num + 1
					self.set_of_states.add(self.current_state)
				else:
					self.transition_function[(self.current_state, self.RE[i])].add(self.state_num)
					i = i + 1
	 
			elif self.RE[i] in self.characters:
				if close_state != 0:
					self.transition_function[(self.current_state, self.RE[i])].add(self.state_num+1)
					self.current_state = self.state_num + 1
					self.state_num = self.state_num + 1
					self.set_of_states.add(self.current_state)
					
					if i == len(self.RE)-1:
						self.transition_function[(self.current_state, 'E')].add(close_state)
						self.current_state = close_state
				else:
					self.transition_function[(self.current_state, self.RE[i])].add(self.state_num+1)
					self.current_state = self.state_num + 1
					self.state_num = self.state_num + 1
					self.set_of_states.add(self.current_state)
	 
			elif self.RE[i] == '+':
				if close_state == 0:
					self.current_state = self.state_num + 1
					self.state_num = self.state_num + 1
					self.set_of_states.add(self.current_state)
					close_state = self.current_state
					self.current_state = initial_state
					self.transition_function[(self.state_num-1, 'E')].add(close_state)
					self.accepting_states.add(self.state_num)
				else:
					self.transition_function[(self.state_num, 'E')].add(close_state)
					self.current_state = initial_state
	 
			elif self.RE[i] == '(':
				sub_initial_state = self.current_state
				i = self.generate_NFA(sub_initial_state, i+1)
				if ((i == len(self.RE)-1 and self.RE[i] == '*') or (self.RE[i] == '*' and i < len(self.RE)-1 and self.RE[i+1] not in self.characters)):
					self.accepting_states.add(self.current_state)
				close_state = 0
			i = i + 1
	
		self.accepting_states.add(self.current_state)
  
	def is_valid_utility(self, ch, state):
		frontier = set()
		if (state, ch) in self.transition_function:
			next_states = self.transition_function[(state, ch)]
			frontier = frontier | next_states
   
		if (state, 'E') in self.transition_function:
			eclose = self.find_epsilon_closure(state)
			temp2 = set()
			if state not in eclose:
				temp2 = temp2 | eclose
			for st in temp2:
				frontier = frontier | self.transition_function[(st, ch)]
	
		return frontier

	def find_epsilon_closure(self, state):
		eclose = set()
		if (state, 'E') in self.transition_function:
			eclose = eclose | self.transition_function[(state, 'E')]
		for st in eclose:
			if (st, 'E') in self.transition_function:
				eclose = eclose | self.transition_function[(st, 'E')]
		return eclose

	def epsilon_transition_on_state(self, states):
		state = set()
		state.add(states)
		if (states, 'E') in self.transition_function:
			for s in self.transition_function[(states, 'E')]:
				state.add(s)
				temp = self.epsilon_transition_on_state(s)
				state.union(temp)
		return state
  
	def is_valid(self, w):
		for i in range(0, len(w)):
			if w[i] not in self.characters:
				return False
		i = 1
		final_states = self.is_valid_utility(w[0], 0)
		fs = final_states
		for st in fs:
			tempo = self.find_epsilon_closure(st)
			final_states = final_states | tempo

		while(len(final_states) > 0 and i < len(w)):
			temp_final = set()
			for state in final_states:
				temp = self.is_valid_utility(w[i], state)
				temp_final = temp_final | temp
			final_states = temp_final
			i = i + 1
   
		temporary = set()
		for state in final_states:
			temps = self.find_epsilon_closure(state)
			temporary = temporary | temps
		final_states = final_states | temporary
  
		for state in final_states:
			if state in self.accepting_states:
				return True
		return False

	# shortest string to any accepting state from a given start state
	def Dijkstra(self, start_state):
		strings = []
		for i in range(0, len(self.set_of_states)):
			strings.append("+")
		strings[start_state] = "" # empty string to itself
  
		Q = Queue()
		Q.put(start_state)
		while not Q.empty():
			state = Q.get()
			for ch in self.characters:
				if (state, ch) in self.transition_function:
					# relax for all out-neighbours
					out_neighbours = set()
					out_neighbours = out_neighbours | self.transition_function[(state, ch)]
					for v in out_neighbours:
						y = strings[state] + ch
						if ch == 'E': 
							y = strings[state]
						z = strings[v]
						if z == '+':
							strings[v] = y
							Q.put(v)
						else:
							if len(strings[v]) > len(y):
								strings[v] = y
	
		it = iter(self.accepting_states)
		first_element = next(it)
		shortest = strings[first_element]
		for accept in self.accepting_states:
			if len(strings[accept]) < len(shortest):
				shortest = strings[accept]
		return shortest

	def print_transitions(self):
		for key, value in self.transition_function.items():
			print(key, value)

	def print_accepting_states(self):
		print("Accepting states are: ", end="")
		print(self.accepting_states)