if [[ $1 = "p" ]]; then
    curl --http1.1 -v -X POST -d '{key:lol}' http://127.0.0.1:20480/hello
else
    curl --http1.1 -v http://127.0.0.1:20480/hello.html
fi