from lark import Lark, Transformer, Tree

# Define the grammar
shell_grammar = """
    start: command
    
    command: subshell ("&&" subshell)*

    subshell: "(" list ")" | list* subshell*
    
    list: pipeline_command ("&&" pipeline_command | "||" pipeline_command)*
    
    pipeline_command: simple_command ("|" simple_command)*
    
    simple_command: redirection* wordlist redirection*
                  | wordlist redirection*
    
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

# 入力例
input_text = "(cat filetxt > outputtxt || echo error) && ( ls | grep test )"
input_text = "( ls | grep test )"
input_text = "cat filetxt > outputtxt || echo error && echo"
input_text = "cat filetxt > outputtxt || echo error && ( ls | grep test )"
try:
    ast = parser.parse(input_text)
    print(ast.pretty())  # 構文木の表示
except Exception as e:
    print(f"エラー: {e}")

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
