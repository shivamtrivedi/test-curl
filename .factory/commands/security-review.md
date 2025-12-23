---
description: Security review of current branch changes against master
argument-hint: [optional: specific files to focus on]
---

Run `git diff master...HEAD` to get all changes in the current branch compared to master.

Perform a security-focused code review on the diff with these checks:

1. **Memory Safety**: Buffer overflows (strcpy, sprintf), use-after-free, double-free, NULL dereferences
2. **Integer Safety (§7.3.1)**: Overflows, underflows, sign conversions
3. **Input Validation**: Unvalidated input, format strings, injection
4. **Resource Management**: Memory/file descriptor leaks
5. **Authentication**: Credential handling, weak crypto
6. **Race Conditions**: TOCTOU issues
7. **API Misuse**: Incorrect curl API usage

$ARGUMENTS

For each issue found, use this comment format:

**[SEVERITY]** Issue title

Guideline: <section reference>

Detailed explanation of the security issue.

Suggested fix:
<concrete recommendation>

Summarize findings by severity at the end.
