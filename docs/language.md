# Language

Quantum Script reads like ES3-era JavaScript: `var`, `function`, objects and
prototypes, no classes, no modules syntax. Most JavaScript of that style runs
unchanged, but some details differ — **read [Differences from
JavaScript](#differences-from-javascript) first**; every row there was checked
against the interpreter.

## Lexical structure

- Source is UTF-8. Comments: `// line` and `/* block */`.
- Statements end with `;` (needed between statements on one line; the XYO
  style also puts `;` after every block: `if (x) { ... };`).
- Identifiers: letters, digits, `_`, `$`.
- Numbers: `42`, `3.14`, `0x1F`. **No exponent form** (`1e3` is a compile
  error).
- Strings: `"..."` or `'...'`, escapes `\n \r \t \\ \" \'`, `\xHH`,
  `\uXXXX` (and surrogate pairs, stored as UTF-8), octal `\101`.
- Keywords: `var function return if else for in of while do switch case
  default break continue try catch finally throw new delete typeof
  instanceof yield this self arguments global true false null undefined NaN
  Infinity`. `call` and `apply` are parsed specially after `.`.

## Values and types

| Type | `typeof(x)` | Test | Notes |
|------|-------------|------|-------|
| undefined | `"undefined"` | `Script.isUndefined(x)` | missing variables, missing arguments, missing properties |
| null | `"null"` | `Script.isNull(x)` | `Script.isNil(x)`: undefined or null |
| Boolean | `"Boolean"` | `Script.isBoolean(x)` | |
| Number | `"Number"` | `Script.isNumber(x)` | IEEE `double`; prints with up to 17 significant digits (`0.1 + 0.2` is `0.30000000000000004`) |
| String | `"String"` | `Script.isString(x)` | immutable bytes, UTF-8; `length` counts bytes |
| Array | `"Array"` | `Script.isArray(x)` | dense, indexes `0..length-1` |
| Object | `"Object"` | `Script.isObject(x)` | properties by name; `for in` order is the order in which the names were first seen by the engine, not the insertion order |
| Function | `"Function"` | `Script.isFunction(x)` | script or native; `Script.isNativeFunction(x)` |
| AssociativeArray | `"AssociativeArray"` | `Script.isAssociativeArray(x)` | `new AssociativeArray()`: any key, insertion order |
| Resource | (type name) | `Script.isResource(x)` | native handle created by an extension (file, socket, ...) |

Truthiness: `undefined`, `null`, `false`, `0`, `NaN` and `""` are false.
`"0"` is true. Conversions are explicit with `Convert.toBoolean`,
`Convert.toNumber`, `Convert.toString`, or the constructors used as
functions: `String(x)`, `Number(x)`, `Boolean(x)`.

## Variables and scope

```javascript
var a = 1, b = 2;
total = 3;               // assignment to an undeclared name creates a global
```

- `var` is **function scoped** (a `var` inside `if` / `for` is visible in the
  whole function). There is no `let` / `const`.
- `global` names the global object (`global.x` is the global variable
  `x`). Top level `this` is an object, but not the global object.
- Closures capture variables, not values: functions created in a loop all
  see the last value of the loop variable.

## Operators

| Group | Operators |
|-------|-----------|
| Arithmetic | `+ - * / %`, unary `-`, `++ --` (prefix and postfix) |
| Assignment | `= += -= *= /= %= &= \|= ^= <<= >>= >>>=` |
| Comparison | `== != === !== < <= > >=` (strings compare by bytes) |
| Logical | `&& \|\| !` — `&&` / `\|\|` return **true / false**, not an operand |
| Bitwise | `& \| ^ ~ << >> >>>` on 64-bit integers (`1 << 40` is `1099511627776`, a shift of 64 or more gives `0` / `-1`) |
| Conditional | `c ? a : b` |
| Other | `typeof(x)`, `x instanceof F`, `"key" in object`, `delete x.y`, `new F(...)`, `.` and `[]` |

`+` concatenates when either side is a string (`"5" + 2` is `"52"`); the other
arithmetic operators convert to number (`"5" * 2` is `10`). `%` follows C
`fmod` (`-7 % 3` is `-1`, `x % 0` is `NaN`).

## Statements

```javascript
if (a > b) { ... } else if (a == b) { ... } else { ... };
for (var i = 0; i < 10; ++i) { ... };
for (var key in object) { ... };     // keys: property names, array indexes (numbers)
for (var value of object) { ... };   // values, also for arrays
while (cond) { ... };
do { ... } while (cond);
switch (x) {
case 1:
	...
	break;                           // without break, falls through
default:
	...
};
break; continue;                     // innermost loop only, no labels
```

Deleting properties while iterating with `for in` / `for of` is allowed; the
loop continues with the next remaining key.

## Functions

```javascript
function add(a, b) {
	return a + b;
};

var mul = function(a, b) {
	return a * b;
};

var factorial = function(n) {
	if (n < 2) {
		return 1;
	};
	return n * self(n - 1);          // self: the function being run
};

function sum() {
	var total = 0;
	for (var k = 0; k < arguments.length; ++k) {   // arguments: an Array
		total += arguments[k];
	};
	return total;
};
```

- Missing arguments are `undefined`; extra ones are in `arguments`.
- **Function declarations are not hoisted**: `function f() {}` is an
  assignment executed where it stands. Call a function only after its
  declaration has run (a function body may refer to a function declared
  later in the file, as long as the call happens after both ran).
- `fn.call(thisValue, a, b)` and `fn.apply(thisValue, [a, b])` work on script
  and native functions. `fn.bind(thisValue, a, ...)` returns a bound
  function.
- A plain call `f()` has `this` = `undefined`; a method call `o.f()` has
  `this` = `o`. A detached method (`var g = o.f; g();`) loses `this`.
- Calls in tail position (`return f(x);`) do not grow the stack.
- Converting a function to a string gives its source (`"" + fn`). A function
  without source (compiled with `--strip`, or passed to
  `Script.protectSource(fn)`) throws `toString` when converted.

### Closures

```javascript
function makeCounter() {
	var count = 0;
	return {
		increment: function() {
			++count;
		},
		value: function() {
			return count;
		}
	};
};
```

An inner function can read and assign the variables and arguments of every
enclosing function.

## Objects and prototypes

```javascript
var point = {x: 1, y: 2, "quoted key": 3};
point.z = 4;
point["x"] = 10;
delete point.y;

function Person(name) {
	this.name = name;
};
Person.prototype.hello = function() {
	return "Hello, " + this.name;
};

function Student(name, school) {
	Person.call(this, name);         // parent constructor
	this.school = school;
};
Student.prototype = new Person();    // inheritance
Student.prototype.hello = function() {
	return "Hi, I study at " + this.school;
};

var s = new Student("Ana", "UPB");
s instanceof Student;                // true
s instanceof Person;                 // true
Script.getPrototypeOf(s) === Student.prototype;   // true
var copy = Object.prototype.create(Person.prototype);   // object with that prototype
```

`length` exists on strings, arrays and associative arrays; a plain object has
no `length`. Array `length` can not be assigned.

## Arrays

```javascript
var list = [1, 2, 3];
list[list.length] = 4;               // append (or list.push(4))
list[10];                            // undefined, does not grow the array
list[6] = 7;                         // grows, the gap is undefined
var sorted = list.sort();            // returns a NEW sorted array
```

Indexes are numbers (or numeric strings); writing a non-numeric index throws.
Methods: see [Standard library](standard-library.md#array).

## Associative arrays

An `AssociativeArray` maps any value (number, string, ...) to a value and
keeps the insertion order:

```javascript
var map = new AssociativeArray();
map["b"] = 1;
map[0.5] = "half";
map.length;                          // 2
for (var key in map) { ... };        // "b", 0.5 - in insertion order
for (var value of map) { ... };
delete map["b"];
```

The `in` operator does not see associative array keys (`"b" in map` is
false); test with `Script.isUndefined(map[key])`.

## Exceptions

```javascript
try {
	throw new Error("disk full");
} catch (e) {
	Console.writeLn(e.message);      // disk full
	Console.writeLn(e);              // Error: disk full
	Console.writeLn(Script.stackTraceWithLevel(e.stackTrace, 4));
} finally {
	cleanup();
};
```

- Any value can be thrown; a non-object value is wrapped, so `catch (e)`
  always has `e.message` (`throw "x"` gives `e.message == "x"`).
- Always create errors with `new`: `throw new Error("x")` or
  `throw(new Error("x"))`. Unlike JavaScript, `Error("x")` without `new`
  does not make an error object: `Error` is a plain script function that
  sets `this.message`, `this` is `undefined` in a plain call, and the call
  itself throws `Error: setPropertyBySymbol` instead of your message.
- Runtime errors are exceptions too (`"f" is not a function`, invalid
  arguments of native functions, include errors).
- `finally` always runs, as in JavaScript: after the end of the `try` or the
  `catch`, and after a `return`, `break`, `continue` or exception in them. A
  `return`, `break`, `continue` or `throw` in the `finally` replaces the
  pending one.
- An exception gets `e.stackTrace` (test with `Script.isStackTrace`).
- Uncaught, it ends the script: the interpreter prints
  `Error: <message>` and the stack trace, exit code 1.

## Coroutines: `yield`

A function that contains `yield` is a **coroutine**: calling it runs until the
next `yield`, which returns a value; the **next call of the same function
resumes** after that `yield`. When the function returns (or ends), the next
call starts it again from the top.

```javascript
function counter() {
	var k = 0;
	while (true) {
		++k;
		yield k;
	};
};
counter();   // 1
counter();   // 2
counter();   // 3

function steps() {
	yield "a";
	yield "b";
	return "done";
};
steps();     // "a"
steps();     // "b"
steps();     // "done"
steps();     // "a" again
```

There are no generator objects: the state belongs to the function value.
Create a new function (for example a closure from a factory) for an
independent sequence. `yield` without a value returns `undefined`; `yield`
works inside loops and `try` blocks; a coroutine called as a method keeps the
`this` of each call.

## Differences from JavaScript

| JavaScript | Quantum Script |
|------------|----------------|
| `typeof x` | `typeof(x)` — parentheses required; results are `"Number"`, `"String"`, `"Boolean"`, `"Object"`, `"Array"`, `"Function"`, `"AssociativeArray"`, `"undefined"`, `"null"` |
| `a \|\| b`, `a && b` return an operand | return `true` / `false`: write `x = Script.isUndefined(y) ? z : y`, not `x = y \|\| z` |
| `!!x` | compile error, write `!(!x)` or `Convert.toBoolean(x)` |
| `null == 0` is false | `null == 0` is **true**; use `===` or `Script.isNull` |
| `1e3`, `1.5e-2` | not supported, write `1000`, `0.015` |
| `"abc"[1]` | `undefined`: use `"abc".substring(1, 1)` or `"abc".getElement(1)` (byte value) |
| `"é".length` is 1 | `2` — strings are UTF-8 bytes |
| `arr.sort()` sorts in place | returns a new sorted array, the source is unchanged |
| `arr.join()` uses `","` | the separator is required, `join()` joins with `"undefined"` |
| `arr.length = 0` | throws; build a new array |
| `x.toString()`, `(5).toFixed(2)` | no methods on numbers: `"" + x`, `String(x)`, `Convert.numberFormat(x, 2)` |
| `fn.toString()` | `"" + fn` |
| `str.replace(a, b)` replaces the first match | replaces **all** occurrences, `a` is plain text (no regular expressions) |
| function declarations are hoisted | not hoisted, see [Functions](#functions) |
| `let`, `const`, `=>`, `class`, template strings, regular expressions, `void`, labels, method shorthand, spread, destructuring | not supported |
| `Math`, `JSON`, `parseInt`, `Date`, ... built in | not in the core: `Script.requireExtension("Math")`, `"JSON"`, `"DateTime"`; `Convert.toNumber` for parsing |
| `print` / `console.log` | `Script.requireExtension("Console"); Console.writeLn(x);` |
| generators return iterator objects | a function with `yield` resumes on each call, see [Coroutines](#coroutines-yield) |
| `"key" in map` | does not work for `AssociativeArray` keys |
| a property named `apply` | `o.apply(...)` is parsed as `Function.apply`: do not name methods `apply` |
| `Number.prototype`, `String(x)` objects | `new Number(5)` is the number `5`; there are no wrapper objects |
