if [[ $1 = "p" ]]; then
    curl --http0.9 -v -X POST -d '{key:lol}' http://127.0.0.1:20480/hello_world
else
    curl --http0.9 -v http://127.0.0.1:20480/hello_world
fi