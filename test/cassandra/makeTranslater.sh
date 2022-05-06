#!/bin/bash

tar -xf pomdp.tar.gz
cd pomdp-solve-5.3
make -j9
cd ..
echo '#!/bin/bash' > translate.sh
echo '' >> translate.sh
echo 'if [ ! -n "$1" ]' >> translate.sh
echo 'then' >> translate.sh
echo '  echo "Usage: $0 <cassandra file>"' >> translate.sh
echo '  exit 65' >> translate.sh
echo 'fi' >> translate.sh
echo '' >> translate.sh
echo './pomdp-solve-5.3/src/pomdp-solve -pomdp $1' >> translate.sh
echo 'mv domain.pddl $1.pddl' >> translate.sh
echo 'echo "file written as $1.pddl"' >> translate.sh

chmod +x translate.sh
