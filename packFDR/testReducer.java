package packFDR;

import java.util.*;
import java.lang.*;
import java.io.IOException;
import java.lang.InterruptedException;

import org.apache.hadoop.mapred.*;
import org.apache.hadoop.mapred.OutputCollector;
import org.apache.hadoop.mapred.Reporter;
import org.apache.hadoop.mapreduce.lib.input.FileSplit;
import org.apache.hadoop.mapreduce.RecordReader;

import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.util.LineReader;
import org.apache.hadoop.fs.*;
import org.apache.hadoop.conf.Configuration;

public class testReducer implements Reducer<Text, IntWritable, Text, IntWritable> {
    public void reduce(Text key, Iterator<IntWritable> iter,
            OutputCollector<Text, IntWritable> output, Reporter reporter)
            throws IOException {
        int count = 0;
        while(iter.hasNext()) {
            count += iter.next().get();
        }
        System.out.println(count);
    }

    public void configure(JobConf job) {
    }

    public void close() {
    }
}