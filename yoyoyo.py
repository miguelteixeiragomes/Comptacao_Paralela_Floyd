import os

os.system("git add .")
os.system('git commit -m "commiting..."')
os.system('git push origin master')
os.system('ssh up201203463@ssh.alunos.dcc.fc.up.pt "cd Documents/Computacao_Paralela_Floyd; git pull origin master; mpicc matrix_floyd.c -o matrix_floyd.exe"')
