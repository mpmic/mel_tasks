#include "vm.h"

#include <iostream>

#include "util.h"


namespace vm {


  vm_state create_vm(bool debug) {
	  vm_state state;

	  // enable vm debugging
	  state.debug = debug;


	  register_instruction(state, "LOAD_CONST", [](vm_state& vmstate, const item_t number) {
		  vmstate.stack.push(number);
		  return true;
	  });

	  register_instruction(state, "PRINT", [](vm_state& vmstate, const item_t /*arg*/) {
		std::cout << vmstate.stack.top() << std::endl;
		return true;
	  });

	  register_instruction(state, "EXIT", [](vm_state& /*arg*/, const item_t /*arg*/) {
		return false;
	  });

	  register_instruction(state, "POP", [](vm_state& vmstate, const item_t /*arg*/) {
		vmstate.stack.pop();
		return true;
	  });




	register_instruction(state, "ADD", [](vm_state& vmstate, const item_t /*arg*/) {
		item_t tos = vmstate.stack.top();
		vmstate.stack.pop();
		item_t tos1 = vmstate.stack.top();
		vmstate.stack.pop();

		vmstate.stack.push(tos+tos1);
		  return true;
	  });

	  register_instruction(state, "DIV", [](vm_state& vmstate, const item_t /*arg*/) {
		item_t tos = vmstate.stack.top();
		vmstate.stack.pop();
		item_t tos1 = vmstate.stack.top();
		vmstate.stack.pop();

		if(tos==0){
		  throw div_by_zero{std::string{"Error: Attempted division by zero"}};
		}
		vmstate.stack.push(tos1/tos);
		  return true;
	  });

	  register_instruction(state, "EQ", [](vm_state& vmstate, const item_t /*arg*/) {
		item_t tos = vmstate.stack.top();
		vmstate.stack.pop();
		item_t tos1 = vmstate.stack.top();
		vmstate.stack.pop();

		if(tos==tos1){
		  vmstate.stack.push(item_t{1});
		}
		else{
		  vmstate.stack.push(item_t{0});
		}
		  return true;
	  });

	  register_instruction(state, "NEQ", [](vm_state& vmstate, const item_t /*arg*/) {
		item_t tos = vmstate.stack.top();
		vmstate.stack.pop();
		item_t tos1 = vmstate.stack.top();
		vmstate.stack.pop();

		if(tos==tos1){
		  vmstate.stack.push(item_t{0});
		}
		else{
		  vmstate.stack.push(item_t{1});
		}
		return true;
	  });

	  register_instruction(state, "DUP", [](vm_state& vmstate, const item_t /*arg*/) {
		item_t tos = vmstate.stack.top();

		vmstate.stack.push(tos);

		return true;
	  });


	register_instruction(state, "JMP", [](vm_state& vmstate, const item_t address) {
	  vmstate.pc = address;
	  return true;
	});

	register_instruction(state, "JMPZ", [](vm_state& vmstate, const item_t address) {

	  item_t tos = vmstate.stack.top();
	  vmstate.stack.pop();

	  if(tos==0){
		vmstate.pc = address;
	  }

	  return true;
	});

	register_instruction(state, "WRITE", [](vm_state& vmstate, const item_t /*arg*/) {
	  vmstate.vm_output_string += std::to_string(vmstate.stack.top());
	  return true;
	});

	register_instruction(state, "WRITE_CHAR", [](vm_state& vmstate, const item_t /*arg*/) {
	  vmstate.vm_output_string += static_cast<char>(vmstate.stack.top());
	  return true;
	});

	  return state;
  }


  void register_instruction(vm_state& state, std::string_view name,
							const op_action_t& action) {
	  size_t op_id = state.next_op_id;


	  state.instruction_ids[static_cast<std::string>(name)] = op_id;
	  state.instruction_names[op_id] = name;
	  state.instruction_actions[op_id] = action;
	  ++state.next_op_id;

  }


  code_t assemble(const vm_state& state, std::string_view input_program) {
	  code_t code;

	  // convert each line separately
	  for (auto& line : util::split(input_program, '\n')) {

		  auto line_words = util::split(line, ' ');

		  // only support instruction and one argument
		  if (line_words.size() >= 3) {
			  throw invalid_instruction{std::string{"more than one instruction argument: "} + line};
		  }

		  // look up instruction id
		  auto& op_name = line_words[0];
		  auto find_op_id = state.instruction_ids.find(op_name);
		  if (find_op_id == std::end(state.instruction_ids)) {
			  throw invalid_instruction{std::string{"unknown instruction: "} + op_name};
		  }
		  op_id_t op_id = find_op_id->second;

		  // parse the argument
		  item_t argument{0};
		  if (line_words.size() == 2) {
			  argument = std::stoll(line_words[1]);
		  }

		  // and save the instruction to the code store
		  code.emplace_back(op_id, argument);
	  }

	  return code;
  }


  std::tuple<item_t, std::string> run(vm_state& vm, const code_t& code) {
	  // to help you to debug the code!
	  auto address_length = code.size();

	  if (vm.debug) {
		  std::cout << "=== running vm ======================" << std::endl;
		  std::cout << "disassembly of run code:" << std::endl;
		  for (const auto &[op_id, arg] : code) {
			  if (not vm.instruction_names.contains(op_id)) {
				  std::cout << "could not disassemble - op_id unknown..." << std::endl;
				  std::cout << "turning off debug mode." << std::endl;
				  vm.debug = false;
				  break;
			  }
			  std::cout << vm.instruction_names[op_id] << " " << arg << std::endl;
		  }
		  std::cout << "=== end of disassembly" << std::endl << std::endl;
	  }

	  // execution loop for the machine
	  while (true) {

		  auto& [op_id, arg] = code[vm.pc];


		  if((op_id == vm.instruction_ids["EXIT"] ||
			  op_id == vm.instruction_ids["PRINT"]||
			  op_id == vm.instruction_ids["POP"]||
			  op_id == vm.instruction_ids["DUP"]||
			  op_id == vm.instruction_ids["WRITE"]||
			  op_id == vm.instruction_ids["WRITE_CHAR"]||
			  op_id == vm.instruction_ids["JMPZ"]) && (vm.stack.size()==0)){
			throw vm_stackfail{"NOT UP IN HERE! "};
		  }

		if((op_id == vm.instruction_ids["ADD"] ||
		op_id == vm.instruction_ids["DIV"]||
		op_id == vm.instruction_ids["EQ"]||
		op_id == vm.instruction_ids["NEQ"]) && (vm.stack.size()<2)){
		  throw vm_stackfail{"NOT UP IN HERE!"};
		}

		if((op_id == vm.instruction_ids["JMP"] ||
			op_id == vm.instruction_ids["JMPZ"]) && (arg>=static_cast<int>(address_length) || arg<0)){
		  throw vm_segfault{"NOT UP IN HERE! SEG FAULT"};
		}


		if (vm.debug) {
			  std::cout << "-- exec " << vm.instruction_names[op_id] << " arg=" << arg << " at pc=" << vm.pc << std::endl;
		  }

		  // increase the program counter here so its value can be overwritten
		  // by the instruction when it executes!
		  vm.pc += 1;

		  auto current_action = vm.instruction_actions[op_id];

		  bool exit_status = current_action(vm, arg);

		  if (not exit_status){
			break;
		  }


	  }

	  auto return_value = std::tuple{vm.stack.top(), vm.vm_output_string};

	  return return_value;
}


} // namespace vm
