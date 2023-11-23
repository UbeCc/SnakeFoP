PWD=$1
OUT_PWD=$2
echo ${PWD} ${OUT_PWD}/SnakeFoP.app/Contents/MacOS FINISHED
cp -r ${PWD}/maps ${OUT_PWD}/SnakeFoP.app/Contents/MacOS
cp -r ${PWD}/config ${OUT_PWD}/SnakeFoP.app/Contents/MacOS
cd ${OUT_PWD}/SnakeFoP.app/Contents/MacOS
if [ ! -d "records" ]; then
	mkdir records
fi
