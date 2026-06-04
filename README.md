# todo
A minimal command-line todo list manager written in C.

## Build
```
cmake -B build
cmake --build build
```

## Usage
```
./todo add "Buy milk" // Add a todo
./todo list           // List all todos
./todo done 2         // Remove todo number 2
./todo clear          // Remove all todos
```

## Notes
Todos are stored in a plain text file called todos.txt in the current directory.