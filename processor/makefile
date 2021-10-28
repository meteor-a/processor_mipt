path_bin=bin
path_exe=exe
path_assembler=./assembler
path_disassembler=./disassembler
path_cpu=./cpu
path_stack_lib=$(path_cpu)/stack_lib
path_text_lib=./text_lib
path_logger=./logger
path_graphics=$(path_cpu)/graphics
name_cpu_exe=cpu.exe
name_cpu_sfml_exe=cpu_sfml.exe
name_asm_exe=asm.exe
name_disasm_exe=disasm.exe

LDFLAGS = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

.PHONY: cpu cpu_sfml asm disasm clean

clean:
	-rm $(path_bin)/*.o

cpu_sfml: $(path_bin)/main_cpu.o $(path_bin)/cpu.o $(path_bin)/graphics.o $(path_bin)/stack_lib.o $(path_bin)/logger.o 
	g++ $(path_bin)/main_cpu.o $(path_bin)/cpu.o $(path_bin)/graphics.o $(path_bin)/stack_lib.o $(path_bin)/logger.o $(LDFLAGS) -o $(path_exe)/$(name_cpu_sfml_exe) 

cpu: $(path_bin)/main_cpu.o $(path_bin)/cpu.o $(path_bin)/graphics.o $(path_bin)/stack_lib.o $(path_bin)/logger.o 
	g++ $(path_bin)/main_cpu.o $(path_bin)/cpu.o $(path_bin)/graphics.o $(path_bin)/stack_lib.o $(path_bin)/logger.o -o $(path_exe)/$(name_cpu_exe) 

asm: $(path_bin)/main_assembler.o $(path_bin)/assembler.o $(path_bin)/logger.o $(path_bin)/text_lib.o
	g++ $(path_bin)/main_assembler.o $(path_bin)/assembler.o $(path_bin)/logger.o $(path_bin)/text_lib.o -o $(path_exe)/$(name_asm_exe) 

disasm: $(path_bin)/main_disassembler.o $(path_bin)/disassembler.o $(path_bin)/logger.o
	g++ $(path_bin)/main_disassembler.o $(path_bin)/disassembler.o $(path_bin)/logger.o -o $(path_exe)/$(name_disasm_exe) 

$(path_bin)/main_cpu.o: $(path_cpu)/main_cpu.cpp
	g++ -c $(path_cpu)/main_cpu.cpp -o $(path_bin)/main_cpu.o
	
$(path_bin)/main_assembler.o: $(path_assembler)/main_assembler.cpp
	g++ -c $(path_assembler)/main_assembler.cpp -o $(path_bin)/main_assembler.o
	
$(path_bin)/main_disassembler.o: $(path_disassembler)/main_disassembler.cpp
	g++ -c $(path_disassembler)/main_disassembler.cpp -o $(path_bin)/main_disassembler.o
	
$(path_bin)/graphics.o: $(path_graphics)/graphics.cpp
	g++ -c $(path_graphics)/graphics.cpp -o $(path_bin)/graphics.o

$(path_bin)/assembler.o: $(path_assembler)/assembler.cpp
	g++ -c $(path_assembler)/assembler.cpp -o $(path_bin)/assembler.o

$(path_bin)/disassembler.o: $(path_disassembler)/disassembler.cpp
	g++ -c $(path_disassembler)/disassembler.cpp -o $(path_bin)/disassembler.o
 
$(path_bin)/cpu.o: $(path_cpu)/cpu.cpp
	g++ -c $(path_cpu)/cpu.cpp -o $(path_bin)/cpu.o -o $(path_bin)/cpu.o

$(path_bin)/logger.o: $(path_logger)/logger.cpp
	g++ -c $(path_logger)/logger.cpp -o $(path_bin)/logger.o

$(path_bin)/stack_lib.o: $(path_stack_lib)/stack_lib.cpp
	g++ -c $(path_stack_lib)/stack_lib.cpp -o $(path_bin)/stack_lib.o
 
$(path_bin)/text_lib.o: $(path_text_lib)/text_lib.cpp
	g++ -c $(path_text_lib)/text_lib.cpp -o $(path_bin)/text_lib.o