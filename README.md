# Разделы

# Русский
[1. Команды](#Команды) 
[Примеры программ](#Примеры программ)

# English
[1. Commands](#Commands)  

## Репозиторий включает в себя: ассемблер, процессор(исполнитель ассемблерного кода), дисассемблер.

### Команды

Команда | кол-во аргументов | описание
--- | --- | ---
HLT | 0 | конец программы
PUSH | 1 | положить элемент в стэк
POP | 1 | достать элемент из стэка
IN | 1 | считать элемент
OUT | 0 | вывести элемент
ADD | 0 | достать два элемента из стэка, сложить их и результат положить обратно
SUB | 0 | достать два элемента из стэка, вычисть их и результат положить обратно
MUL | 0 | достать два элемента из стэка, умножить их и результат положить обратно
DIV | 0 | достать два элемента из стэка, разделить(целочисленно) их и результат положить обратно
SQRT | 0 | достать элемент из стека, вычислить его квадратный корень и положить результат обратно
JMP | 1 | перейти к метке без условия
JA | 1 | достать два элемента из стека, сравнить их (более глубокий > менее глубокий).  Если true, то перейти к метке, иначе продолжить выполнение
JAE | 1 | достать два элемента из стека, сравнить их (более глубокий >= менее глубокий). Если true, то перейти к метке, иначе продолжить выполнение
JB | 1 | достать два элемента из стека, сравнить их (более глубокий < менее глубокий).  Если true, то перейти к метке, иначе продолжить выполнение
JBE | 1 | достать два элемента из стека, сравнить их (более глубокий <= менее глубокий). Если true, то перейти к метке, иначе продолжить выполнение
JE | 1 | достать два элемента из стека, сравнить их (более глубокий == менее глубокий). Если true, то перейти к метке, иначе продолжить выполнение
JNE | 1 | достать два элемента из стека, сравнить их (более глубокий != менее глубокий). Если true, то перейти к метке, иначе продолжить выполнение
AH | 0 | развлекательная команда)0)
CALL | 1 | вызов функции(переход к метке)
RET | 0 | возврат из функции и продолжение выполнение программы, после call
GRAPH | 0 | отрисовка графики
STROUT | 1 | вывод строки
DB | 1 | обозначение строки

### Примеры программ
Примеры находятся в папке examples

### Синтаксис команд

1. **PUSH**:  

Команда | описание
--- | ---
PUSH 5 | положить число 5 в стэк  
PUSH [5] | положить в стек число из 5 ячейки ОЗУ  
PUSH ax | положить в стэк значение регистра ax  
PUSH [ax] | положить в стек число из ячейки ОЗУ с номером, лежащем в ax (ОЗУ[ax])  
PUSH 5+ax | положить в стек число, полученно в результате сложения 5 и числа из регистра ax  
PUSH ax+5 | положить в стек число, полученно в результате сложения 5 и числа из регистра ax   
PUSH 5.23 | положить число 5.23 в стэк  

2. **POP**:  

Команда | описание
--- | ---
POP ax | достать число из стека и положить его в регистр ax  
POP [5] | достать число из стека и положить его в ячейку 5 ОЗУ  
POP [ax+5] | достать число из стека и положить его в ячейку ОЗУ с номером, ax + 5  
POP [5+ax] | достать число из стека и положить его в ячейку ОЗУ с номером, ax + 5  

3. **IN**:  

Команда | описание
--- | ---
IN ax | считать число и положить его в регистр ax  
IN [5] | считать число и положить его в ячейку 5 ОЗУ  
IN [ax+5] | считать число и положить его в ячейку ОЗУ с номером, ax + 5  
IN [5+ax] | считать число и положить его в ячейку ОЗУ с номером, ax + 5  

4. **JMP**/**JA**/**JAE**/**JB**/**JBE**/**JE**/**JNE**/**CALL**:  
**Команда** *метка*  

5. Примеры **Метки**:  

Команда | описание
--- | ---
Label | метка
Cur | метка
DFG | метка
bv | метка

6. Объявление **Метки**:  
Label:  
Cur:  
DFG:  
bv:  

7. **DB**:  
DB $text$  

8. Объявление строки:  
TextAboutAhatina:  
  DB $text$  
  
9. Вывод строки:
STROUT TextAboutAhatina

## Компиляция ассемблера:  
> make asm

## Компиляция дисассемблера:  
> make disasm

## Компиляция процессора:  
### Windows
> make cpu

### Linux
> make cpu_sfml

## Графическая часть:  
Графическая память начинается с 1024 элемента, размера 512
### Windows  
Отрисовка в консоли(все, что не равно 0, отрисовываем в виде *)  

### Linux
Отрисовка с использованием SFML(фон красный, все, что не ноль, рисуется зеленым цветом)

## Ошибка синтаксиса в ассемблере
Об ошибке информируется в консоле и в файл log_syntax.txt c указанием строки и команды

## Запуск ассемблера
  > ./exe/asm.exe

## Запуск процессора
### Windows
  > ./exe/cpu.exe

### Linux
  > ./exe/cpu_sfml.exe

## Запуск дисассемблера
  > ./exe/disasm.exe

## Диссассемблер  
Необходим для расшифровки ассемблерного кода в "человеческий". При это метки заменяются на позицию в ассемблерном коде

# English

## The repository includes: assembler, processor (assembler code executor), disassembler.

### Commands:

Command | number of arguments | description
--- | --- | ---
HLT | 0 | end of program
PUSH | 1 | put an item on the stack
POP | 1 | get an element from the stack
IN | 1 | count element
OUT | 0 | output element
ADD | 0 | get two elements from the stack, add them up and put the result back
SUB | 0 | get two elements from the stack, clean them and put the result back
MUL | 0 | get two elements from the stack, multiply them and put the result back
DIV | 0 | get two elements from the stack, divide them (integer) and put the result back
SQRT | 0 | get an element from the stack, calculate its square root and put the result back
JMP | 1 | go to label without condition
JA | 1 | get two elements from the stack, compare them (deeper > less deep). If true, then go to the label, otherwise continue execution
JAE | 1 | get two elements from the stack, compare them (deeper >= less deep). If true, then go to the label, otherwise continue execution
JB | 1 | get two elements from the stack, compare them (deeper < less deep). If true, then go to the label, otherwise continue execution
JBE | 1 | get two elements from the stack, compare them (deeper <= less deep). If true, then go to the label, otherwise continue execution
JE | 1 | get two elements from the stack, compare them (deeper == less deep). If true, then go to the label, otherwise continue execution
JNE | 1 | get two elements from the stack, compare them (deeper!= less deep). If true, then go to the label, otherwise continue execution
AH | 0 | entertainment team)0)
CALL | 1 | function call(transition to label)
RET | 0 | return from function and continue program execution, after call
GRAPH | 0 | drawing graphics
STROUT | 1 | line output
DB | 1 | string designation

### Sample programs
Examples are located in the examples folder

### Command syntax

1. **PUSH**:

Command | Description
--- | ---
PUSH 5 | put the number 5 on the stack
PUSH [5] | put a number of 5 RAM cells on the stack
PUSH ax | put the value of the ax register on the stack
PUSH [ax] | put on the stack a number from a RAM cell with a number lying in ax (RAM[ax])
PUSH 5+ax | put a number on the stack resulting from the addition of 5 and a number from the ax register
PUSH ax+5 | put a number on the stack resulting from the addition of 5 and a number from the ax register
PUSH 5.23 | put the number 5.23 on the stack

2. **POP**:

Command | Description
--- | ---
POP ax | get a number from the stack and put it in the ax register
POP [5] | get a number from the stack and put it in cell 5 of RAM
POP [ax+5] | get a number from the stack and put it in the RAM cell with the number, ax + 5
POP [5+ax] | get a number from the stack and put it in the RAM cell with the number, ax + 5

3. **IN**:

Command | Description
--- | ---
IN ax | count a number and put it in the ax register
IN [5] | count a number and put it in cell 5 of RAM
IN [ax+5] | count a number and put it in the RAM cell with the number, ax + 5
IN [5+ax] | count the number and put it in the RAM cell with the number, ax + 5

4. **JMP**/**JA**/**JAE**/**JB**/**JBE**/**JE**/**JNE**/**CALL**:
**Command** *label*

5. Examples of **Labels**:

Command | Description
--- | ---
Label | Label
Cur | label
DFG | label
bv | label

6. Ad **Tags**:
Label:
Cur:
DFG:
bv:

7. **DB**:
DB $text$

8. Line Declaration:
TextAboutAhatina:
DB $text$

9. Line output:
STROUT TextAboutAhatina

## Assembler compilation:
> make asm

## Disassembler compilation:
> make disasm

## CPU Compilation:
### Windows
> make cpu

### Linux
> make cpu_sfml

## Graphic part:
The graphics memory starts with 1024 elements, size 512
### Windows
Rendering in the console (everything that is not equal to 0 is rendered as *)

### Linux
Rendering using SFML (the background is red, everything that is not zero is drawn in green)

## Syntax error in assembler
The error is reported in the console and in the file log_syntax.txt specifying the line and command

## Starting the assembler
> ./exe/asm.exe

## Starting the processor
### Windows
> ./exe/cpu.exe

### Linux
> ./exe/cpu_sfml.exe

## Starting the disassembler
> ./exe/disasm.exe

## Disassembler
It is necessary for decoding the assembler code into "human". In this case, the labels are replaced by a position in the assembly code
