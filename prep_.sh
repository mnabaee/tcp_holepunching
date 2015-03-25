g++ server.cc -o server -lpthread
g++ client.cc -o client -lpthread
scp -i ../mahdy.pem -r ./ ubuntu@54.68.127.121:~/pp/
ssh -i ../mahdy.pem ubuntu@54.68.127.121 'g++ ~/pp/server.cc -o ~/pp/server -lpthread'
ssh -i ../mahdy.pem ubuntu@54.68.127.121 'g++ ~/pp/client.cc -o ~/pp/client -lpthread'
