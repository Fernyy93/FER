## Github Cheatsheet
# GitHub Command Cheatsheet

## Basic Git Commands

### Setting Up

# Configure user information
git config --global user.name "Your Name"
git config --global user.email "you@example.com"

# Initialize a new git repository
git init

# Clone an existing repository
git clone https://github.com/user/repo.git

# Check status of your files in the working directory
git status

# Add changes to the staging area
git add <filename>
git add .

# Commit changes to the local repository
git commit -m "Commit message"

# List all branches
git branch

# Create a new branch
git branch <branch-name>

# Switch to a branch
git checkout <branch-name>

# Create and switch to a new branch
git checkout -b <branch-name>

# Merge a branch into the current branch
git merge <branch-name>

# List remote repositories
git remote -v

# Add a new remote repository
git remote add origin https://github.com/user/repo.git

# Push changes to a remote repository
git push origin <branch-name>

# Pull changes from a remote repository
git pull origin <branch-name>

# Show commit history
git log

# Show commit history with diffs
git log -p

# Show commit history as a graph
git log --graph --oneline --decorate --all

# Unstage changes
git reset <filename>

# Revert a commit
git revert <commit-hash>

# Reset to a previous commit
git reset --hard <commit-hash>

# Create a new tag
git tag <tag-name>

# Push tags to remote
git push origin --tags

# List tags
git tag

# Add a submodule
git submodule add https://github.com/user/repo.git

# Initialize submodules
git submodule init

# Update submodules
git submodule update

# Create a fork of a repository on GitHub
# (Done through the GitHub web interface)

# Create a pull request
# (Done through the GitHub web interface)

# Open an issue
# (Done through the GitHub web interface)

# Create a project board
# (Done through the GitHub web interface)


