package packFDR;

import java.util.*;
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

public class testMapper implements Mapper<LongWritable, Text, Text, IntWritable> {
    public void map(LongWritable key, Text value, OutputCollector<Text, IntWritable> output,
            Reporter reporter) throws IOException {
        String lines = value.toString();
        String []lineArr = lines.split("\n");
        int lcount = lineArr.length;
        System.out.println(key + " : " + value);
    }

    public void configure(JobConf job) {
    }

    public void close() {
    }
}