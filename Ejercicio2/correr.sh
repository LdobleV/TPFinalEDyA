if [ -f feedback ]; then
    rm feedback
fi
make sensor -s && \
mkfifo feedback && \
(./robot < feedback | ./sensor $1 > feedback ; \
rm feedback)