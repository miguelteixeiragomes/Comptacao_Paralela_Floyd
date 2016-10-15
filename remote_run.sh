git add .
git commit -m "commiting..."
git push origin master
ssh up201203463@ssh.alunos.dcc.fc.up.pt "cd Documents/Computacao_Paralela_Floyd; git stash; git pull origin master; mpicc matrix_floyd.c -o matrix_floyd.exe"