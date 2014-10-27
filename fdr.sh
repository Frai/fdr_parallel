# system configuration
#hadoop_bin=/home/antoniocf/hadoop-2.4.1/bin/hadoop # path to the hadoop executable
#base_path=/home/antoniocf/fdr_parallel # complete path to the 'BoW_package' folder
hadoop_bin=/Users/frai/Programs/hadoop-2.4.1/bin/hadoop # path to the hadoop executable
base_path=/Users/frai/Programs/fdr_parallel # complete path to the 'BoW_package' folder
m=1 # number of mappers to be used (set with the number automatically defined by Hadoop)
r=1 # number of reducers to be used

# dataset 1 configuration
dataset=synthetic_hadoop.dat # dataset name
dimensionality=4 # dataset dimensionality
size=100000 # dataset size (total number of points)

# dataset 2 configuration
# dataset=synthetic_hadoop_2.dat # dataset name
# dimensionality=2 # dataset dimensionality
# size=4 # dataset size (total number of points)

# dataset 3 configuration
#dataset=synthetic_hadoop_3.dat # dataset name
#dimensionality=78 # dataset dimensionality
#size=150000 # dataset size (total number of points)

# dataset 4 configuration
# dataset=synthetic_hadoop_4.dat # dataset name
# dimensionality=2 # dataset dimensionality
# size=5 # dataset size (total number of points)

# dataset 5 configuration
# dataset=synthetic_hadoop_5.dat # dataset name
# dimensionality=3 # dataset dimensionality
# size=1000000 # dataset size (total number of points)

# dataset 6 configuration
#dataset=synthetic_hadoop_6.dat # dataset name
#dimensionality=3 # dataset dimensionality
#size=1000 # dataset size (total number of points)

# dataset 7 configuration
# dataset=synthetic_hadoop_7.dat # dataset name
# dimensionality=15 # dataset dimensionality
# size=100000 # dataset size (total number of points)

# dataset 8 configuration
#dataset=synthetic_hadoop_8.dat # dataset name
#dimensionality=3 # dataset dimensionality
#size=100000 # dataset size (total number of points)

# dataset 9 configuration
# dataset=synthetic_hadoop_9.dat # dataset name
# dimensionality=15 # dataset dimensionality
# size=10 # dataset size (total number of points)

input=hdfs/$dataset/ # complete path in the HDFS file system to the data files

# initial wall-clock time meassurement
# initialTime="$(date +%s)"

echo "Compiling jar..."
#javac -source 1.6 -target 1.6 -bootclasspath /usr/lib/jvm/java-1.6.0-openjdk-1.6.0.0.x86_64/jre/lib/rt.jar packFDR/RecordReaderFDR.java packFDR/InputFormatFDR.java -cp hadoop-core-1.2.1.jar
javac packFDR/RecordReaderFDR.java packFDR/InputFormatFDR.java -cp hadoop-core-1.2.1.jar
jar cf libFDR.jar packFDR

cd $base_path/ParC
# make spotless
make

rm -f ../result_tmp.txt
rm -f dimensionality
echo $dimensionality > dimensionality
rm -f size
echo $size > size
rm -f divisions
echo $r > divisions

$hadoop_bin fs -rm -r ParC
$hadoop_bin fs -mkdir ParC

rm -r -f ../results/$dataset
mkdir -p ../results/$dataset/output_parallel_$r

dimensions="dimensions"
dimensions_tmp="dimensions_tmp"
cp $dimensions $dimensions_tmp

# Running for the first time.
# Parameters:
# - The file access mode is w (write) to create a new file.
# - The index number is 0, so all the dimensions are taken in consideration
echo "Running Hadoop..."
./genDimensions w $dimensionality
$hadoop_bin jar ../myHadoopStreaming.jar -libjars ../libFDR.jar -D mapreduce.task.timeout=0 fs.local.block.size=1048576 -inputformat packFDR.InputFormatFDR -mapper mapper -reducer reducer -file mapper -file reducer -file dimensionality -file size -file divisions -file parameters -file dimensions -file dimensions_tmp -input $input -output ParC/output/ -numReduceTasks $r
$hadoop_bin fs -get ParC/output/* ../results/$dataset/output_parallel_$r/
$hadoop_bin fs -rm -r ParC
$hadoop_bin fs -getmerge ../results/$dataset/output_parallel_$r/ ../results/$dataset/output_parallel_$r/merged.txt
./merge w $(wc -l ../results/$dataset/output_parallel_$r/merged.txt)
echo "Finished first run..."
cat ../result.txt
# End of the first run.

# Begin of the following runs
# Parameters:
# - The file access mode is a (append) to append the existing file.
# - The index number will change as the following algorithm:
#   - i from dimensionality to one.
#   - j from one to dimensionality
#echo "Started" $dimensionality "runs..."
#i=$dimensionality
#num=1
#while [ "$i" -ge "$num" ];
#do
#    echo "i $i"
#    j=1
#    while [ "$j" -le "$i" ];
#    do
#        echo "j $j"
#        $hadoop_bin fs -rm -r ParC
#        $hadoop_bin fs -mkdir ParC
#
#        rm -r -f ../results/$dataset
#        mkdir -p ../results/$dataset/output_parallel_$r
#
#        # Changes the file of the dimensions to be considered.
#        ./genDimensions a $dimensionality
#
#        # Run Hadoop with the selected dimensions
#        $hadoop_bin jar ../myHadoopStreaming.jar -libjars ../libFDR.jar -D mapreduce.task.timeout=0 fs.local.block.size=1048576 -inputformat packFDR.InputFormatFDR -mapper mapper -reducer reducer -file mapper -file reducer -file dimensionality -file size -file divisions -file parameters -file dimensions -file dimensions_tmp -input $input -output ParC/output/ -numReduceTasks $r
#
#        $hadoop_bin fs -get ParC/output/* ../results/$dataset/output_parallel_$r/
#        $hadoop_bin fs -rm -r ParC
#        $hadoop_bin fs -getmerge ../results/$dataset/output_parallel_$r/ ../results/$dataset/output_parallel_$r/merged.txt
#        ./merge a $(wc -l ../results/$dataset/output_parallel_$r/merged.txt)
#
#        j=$(($j+1))
#    done
#
#    # Check which one is the smallest updates the index
#    ./updateDimensions $dimensionality
#
#    i=$(($i-1))
#done
#echo "Finished all the runs..."
# End of the loop

# final wall-clock time meassurement
# totalTime=`echo "$(date +%s) - $initialTime" | bc -l`
# echo $totalTime > ../results/$dataset/output_parallel_$r/time

#cleanup
echo "Cleaning..."
make spotless
rm -f dimensionality
rm -f size
rm -f divisions
rm -f sample
rm -f part-00000
