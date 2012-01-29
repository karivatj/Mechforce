if lsb_release -c | egrep -q "lucid|maverick|natty|oneiric"; then
     which aptitude > /dev/null 2>&1 || sudo apt-get install aptitude
	 sudo aptitude -y install libsdl1.2-dev libsdl-mixer1.2-dev libsdl-image1.2-dev \
	 libftgl-dev libfreetype6-dev
fi
