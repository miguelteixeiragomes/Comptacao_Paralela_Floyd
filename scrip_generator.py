import os
import glob

os.system("git add .")
os.system('git commit -m "commiting..."')
os.system('git push origin master')


f = open('remote_script', 'w')
f.write("cd ~/Documents/Computacao_Paralela_Floyd\ngit pull origin master")
f.close()

os.system('ssh up201203463@ssh.alunos.dcc.fc.up.pt sh -c "$(<remote_script)"')

os.remove('remote_script')
