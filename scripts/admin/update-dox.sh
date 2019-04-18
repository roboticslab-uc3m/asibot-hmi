#!/bin/sh

# Thanks: http://stackoverflow.com/questions/14710257/running-a-cron-job-at-230-am-every-day
# On how to automate process at 2:30 every day (type "date" to get your server time)
# crontab -e
# 30 2 * * * /your/command

path="$HOME/asibot-hmi"
echo "Update asibot-hmi..."
git -C "$path" pull
echo "Doxy asibot-hmi..."
path="$path/doc/build"
mkdir -p "$path"
make -C "$path" clean && make -C "$path" dox
