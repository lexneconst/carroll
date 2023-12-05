git init
git config --global user.email "songkeart@gmail.com"
git config --global user.name "lexneconst"
git add . 
git add --all
git status
git commit -am 'carroll v1.09x'
git remote add origin 'https://github.com/lexneconst/carroll.git'
git pull --rebase origin
git push origin 'main'

