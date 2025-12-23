---
description: Update documentation based on branch changes against master
argument-hint: [optional: specific doc files to update]
---

Run `git diff master...HEAD` to get all changes in the current branch compared to master.

Based on the diff:

1. **Analyze README.md**: Review the current README.md and determine what sections need updating based on the code changes. Propose specific updates.

2. **Scan other .md files**: Find all other markdown files in the repository using glob patterns. For each file, determine if the changes in the diff affect its content.

3. **Interactive updates**: For each .md file that may need updates, ask if I want to update it before making changes.

$ARGUMENTS

Output format:
- List all .md files found
- For each file, indicate: "Needs update" or "No changes needed" with reasoning
- For README.md, provide the proposed changes directly
- Wait for confirmation before updating other .md files
