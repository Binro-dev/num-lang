NUM Programming Language

A minimalistic, cell-based esoteric programming language designed for fast learning, experimentation, and low-level manipulations.

Version: 1.0.0

Installation

Installation Process

```bash
python install.py
```

Installation Locations

· Linux/macOS: /usr/local/bin/num
· Windows: %LOCALAPPDATA%\Programs\num\num.exe
    Remember to add this path to your PATH environment variable

Uninstallation

```bash
python install.py --uninstall
```

Running NUM Programs

Command Description
```  
num -f filename.num // Run a NUM file
echo "<code>" | num // Quick execution from command line
num --version // check NUM version
num --help // for more info about the usage
```

Language Basics

NUM uses a cell-based memory model with a pointer and global register:

· Cells: Array of integer values (like bytes)
· Pointer: Points to current cell position
· Global Register num: Global value for arithmetic operations

Value Notation [n]

· Optional numeric parameter for opcodes
· Can be:
  · Literal number (e.g., 5)
  · Current cell value ($)
  · Global num value (&)

Opcode Reference Table

```
Opcode | Parameter [n] | Description| Example Usage
`0` ❌ `Forbidden Clear global num value 0`
`1` ✅ `Optional Increment current cell value 1, 1[5], 1[$], 1[&]`
`2` ✅ `Optional Decrement current cell value 2, 2[10], 2[$], 2[&]`
`3` ❌ `Forbidden Clear current cell (set to 0) 3`
`4` ❌ `Forbidden Display current cell as ASCII char 4`
`5` ❌ `Forbidden Read char into current cell 5`
`6` ✅ `Required Loop with counter n 6[10](...)`
`7` ✅ `Optional Move pointer forward 7, 7[5]`
`8` ✅ `Optional Move pointer backward 8, 8[3]`
`9` ✅ `Required Jump to cell n 9[100]`
`+` ✅ `Optional Add to global num +, +[5], +[$], +[&]`
`-` ✅ `Optional Subtract from global num -, -[8], -[$], -[&]`
`*` ✅ `Optional Multiply global num *, *[2], *[$], *[&]`
`/` ✅ `Optional Divide global num (integer division) /, /[4], /[$], /[&]`
```

Detailed Opcode Examples

Arithmetic Operations

Addition (+)

```num
++++++++++    # num += 10 (10 '+' operations)
+[10]         # num += 10
+[$]          # num += current cell value
+[&]          # num += num itself
```

Subtraction (-)

```num
----------    # num -= 10 (10 '-' operations)
-[10]         # num -= 10
-[$]          # num -= current cell value
-[&]          # num -= num itself
```

Multiplication (*)

```num
**********    # num *= 1 (10 '*' operations)
*[10]         # num *= 10
*[$]          # num *= current cell value
*[&]          # num *= num itself
```

Division (/)

```num
//////////    # num /= 1 (10 '/' operations)
/[10]         # num /= 10
/[$]          # num /= current cell value
/[&]          # num /= num itself
# Note: Integer division only - rounds to nearest integer
```

Cell Operations

Increment Cell (1)

```num
1             # cell += 1
1[10]         # cell += 10
1[$]          # cell += current cell value
1[&]          # cell += num value
```

Decrement Cell (2)

```num
2             # cell -= 1
2[10]         # cell -= 10
2[$]          # cell -= current cell value
2[&]          # cell -= num value
```

Clear Operations

```num
0             # Clear global num (num = 0)
3             # Clear current cell (cell = 0)
```

I/O Operations

Output (4)

```num
1[72]         # Set cell to 72 ('H' in ASCII)
4             # Display 'H'
3             # Clear cell
1[10]         # Set cell to 10 ('\n' in ASCII)
4             # Display newline
```

Input (5)

```num
5             # Read character into current cell
```

Pointer Operations

Move Forward (7)

```num
7             # Move to next cell
7[10]         # Move forward 10 cells
```

Move Backward (8)

```num
8             # Move to previous cell
8[10]         # Move backward 10 cells
```

Direct Jump (9)

```num
9[100]        # Jump directly to cell 100
```

Looping (6)

```num
6[10](
    # Code here repeats 10 times
    # Loop breaks if cell value < 0 after completion
)
```

Important Notes

· Cell Values: Integer values only
· Pointer: Cannot go negative (throws error)
· Division: Integer division only, no floating point
· Loops: Break condition - cell value < 0 after loop completion
· Combinations: Opcodes can be combined with loops for complex logic

Sample Programs

Check the test/ folder for comprehensive examples.

---

NUM - Where minimalism meets computational power
