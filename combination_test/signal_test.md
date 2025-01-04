
## Test SIGINT (Ctrl+C) while the Shell is **Idle**

### Goal
When you press **Ctrl+C** at the idle prompt, 
a typical Bash-like shell **does not** exit. 
Instead, it **resets** the input line and shows a new prompt.

1. At the `minishell$` prompt, **do nothing**—just press **Ctrl+C**.
2. Observe the behavior:
   - Does the shell exit? (It **shouldn’t** in a Bash-like shell.)
   - Does it produce a new prompt on a fresh line (like Bash does)?

**Expected Outcome (like Bash)**:
- The shell stays running.
- Your prompt resets to a new line, typically with no partial input.  
- If you do `echo $?` afterward in Bash, you might see `130`, 
   but some mini-shells don’t store that status for an *idle* press. 
   The main point is that the shell is **still alive**.

---

## Test SIGINT (Ctrl+C) **While a Child Process Is Running**

### Goal
When you run an external command in the foreground, 
pressing **Ctrl+C** should kill the **child** process, 
and return control to the shell with an exit code 130.

1. Type a **long-running command** (e.g., `sleep 10`) :
2. Immediately press **Ctrl+C**.
3. Observe:
   - The `sleep 10` command should terminate right away.
   - minishell should show the prompt again (not exit).
   - If you check the shell’s stored exit code
      in Bash, `echo $?` right afterward would be `130`)
      you can see if your shell mimics that behavior.

**Expected Outcome**:
- Child (`sleep 10`) is killed by SIGINT.
- Shell remains alive and returns promptly to a new `minishell$` prompt.
- Shell’s last status might be set to `130` (`128 + 2` for SIGINT).

---

## Test SIGQUIT (Ctrl+\)

### Goal
In Bash-like shells, pressing **Ctrl+\** at the idle prompt **often does nothing** (ignored). 
But if a child is running in the foreground, 
sending **SIGQUIT** typically kills it with a core dump (exit code `131`).

1. **Idle** test:
   - At `minishell$`, press **Ctrl+\**.
   - In Bash, usually nothing happens (the shell ignores SIGQUIT). 
   Check your shell: does it stay alive and just reprint the prompt or do nothing?

2. **Foreground child** test:
   - Type `sleep 10` again at your minishell prompt.
   - Press **Ctrl+\**.
   - Typically, that kills the child and prints something like “Quit (core dumped)” in Bash. 

**Expected Outcome**:
- If you do nothing special, the child is killed with exit code `131` (`128 + 3`) .  
- The parent shell should remain alive.

---

## Verify the Exit Status (If Implemented)
<!-- 
If your shell stores the last command’s exit status in something like `$?`, 
you can test it by running something like:

1. **After** you kill `sleep 10` with Ctrl+C:
   ```bash
   minishell$ echo $?
   ```
   or however you retrieve your last status in your mini-shell. 
   - You might expect **130**.

2. **After** you kill `sleep 10` with Ctrl+\:
   ```bash
   minishell$ echo $?
   ```
   - You might expect **131**.
-->

---

**Foreground child** + Ctrl+C
**Run** `sleep 10` → Press **Ctrl+C** → Child should die
   shell returns prompt and sets status = 130.

**Foreground child** + Ctrl+\
**Run** `sleep 10` → Press **Ctrl+\** → Child should die (often core dump)
   shell returns prompt and sets status = 131.
