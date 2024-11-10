from lark import Lark, Transformer, Tree

# Define the grammar
# shell_grammar = """
#     start: command
#     command: subshell ("&&" subshell)*
#     subshell: "(" list ")" | list* subshell*
    
#     list: pipeline_command ("&&" pipeline_command | "||" pipeline_command)*
    
#     pipeline_command: simple_command ("|" simple_command)*
    
#     simple_command: redirection* wordlist redirection*
#                   | wordlist redirection*
    
#     wordlist: WORD | wordlist WORD
    
#     redirection: ">" filename
#                | "<" filename
#                | ">>" filename
#                | "<<" filename
#                | "<>" filename
#                | NUMBER ">" filename
#                | NUMBER "<" filename
#                | NUMBER ">>" filename
#                | NUMBER "<<" filename
#                | NUMBER "<>" filename
    
#     filename: WORD
#     WORD: /[a-zA-Z0-9_"]+/
#     NUMBER: /[0-9]+/
    
#     %ignore " "
# """

shell_grammar = """
    start: command
	command: cmd_type command_tail
	command_tail: "|" cmd_type command_tail 
            | ("&&" | "||") cmd_type command_tail
            | redirection
			|
	cmd_type: simple_command | subshell
	subshell: "(" command ")" command_tail*
	simple_command: redirection* wordlist redirection* | redirection+
    wordlist: WORD | wordlist WORD
    redirection: ">" filename
               | "<" filename
               | ">>" filename
               | "<<" filename
               | "<>" filename
               | NUMBER ">" filename
               | NUMBER "<" filename
               | NUMBER ">>" filename
               | NUMBER "<<" filename
               | NUMBER "<>" filename
	
    
    filename: WORD
    WORD: /[a-zA-Z0-9_"]+/
    NUMBER: /[0-9]+/
    
    %ignore " "
"""




# Parse and transform the AST
class ShellTransformer(Transformer):
    def command(self, args):
        return Tree("command", args)

    def shell_command(self, args):
        return Tree("shell_command", args)

    def subshell(self, args):
        return Tree("subshell", args)

    def list(self, args):
        return Tree("list", args)

    def pipeline_command(self, args):
        return Tree("pipeline_command", args)

    def simple_command(self, args):
        return Tree("simple_command", args)

    def wordlist(self, args):
        return Tree("wordlist", args)

    def redirection(self, args):
        return Tree("redirection", args)

    def filename(self, args):
        return args[0]

    def WORD(self, token):
        return str(token)

    def NUMBER(self, token):
        return int(token)

parser = Lark(shell_grammar, parser="earley")


input_text = "( ls | grep test )"
input_text = "cat filetxt > outputtxt || echo error && echo"
input_text = "cat filetxt > outputtxt || echo error && ( ls | grep test )"
input_text = "(cat filetxt > outputtxt || echo error) && ( ls | grep test )"
# input_text = "cat filetxt > outputtxt | echo error  | grep test > out < infile > e > o"
commands = [
    "ls",
    "(cat file1 && echo done) > output",
    "(ls | grep test) > result > iyt",
    "ls | grep sample || echo error",
    "(cat file1 && cat file2) || (echo file not found)",
    "(ls | grep log) && (cat logs > log_backup)",
    "cat file1 | sort | uniq || echo failed",
	"< file1 cat | sort abc asfd ec >out | abc |((ls)&&(cat)) >asf || cat >qwe>as>asdf>asf",
	"((ls))",
	">files",
	">files>a>b>c"
]

index = 0
while index < len(commands):
    input_text = commands[index]
    print(f"\n解析中のコマンド: {input_text}")
    
    try:
        ast = parser.parse(input_text)
        print(ast.pretty())
    except Exception as e:
        print(f"エラー: {e}")
    
    index += 1

## Create the parser
#parser = Lark(shell_grammar, parser='earley', transformer=ShellTransformer())

## Example input to test
#input_text = "cat file > output || echo error && ( ls | grep test )"

## Parse the input and generate the AST
#try:
#    ast = parser.parse(input_text)
#    print(ast.pretty())
#except Exception as e:
#    print(f"Error: {e}")
