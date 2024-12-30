#!/bin/sh
urls=(
	"/../etc/password"
	"/../../../../etc/password"
	"/index.html/../../../../../etc/password"
	"/../../../etc/password"
)

for url in "${urls[@]}";do
	echo "Testing $url"
	curl -v "http://localhost:8080$url"
	echo "---------------------------"
done
