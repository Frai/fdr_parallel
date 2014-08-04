package packFDR;

import java.util.*;
import java.lang.*;
import java.io.IOException;
import java.lang.InterruptedException;

import org.apache.hadoop.mapreduce.*;
import org.apache.hadoop.mapreduce.lib.input.*;
import org.apache.hadoop.mapreduce.lib.output.*;
import org.apache.hadoop.mapreduce.lib.input.FileSplit;
import org.apache.hadoop.mapreduce.RecordReader;

import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.util.LineReader;
import org.apache.hadoop.fs.*;
import org.apache.hadoop.conf.Configuration;

import org.apache.hadoop.util.*;
import org.apache.hadoop.conf.Configured;

public class wordcount extends Configured implements Tool{
   public static void main(String[] args) throws Exception {
      Configuration configuration = new Configuration();
      ToolRunner.run(configuration, new wordcount(),args);
   }

   @Override
   public int run(String[] arg0) throws Exception {
      Job job = new Job();
      job.setOutputKeyClass(Text.class);
      job.setOutputValueClass(IntWritable.class);
      job.setJarByClass(wordcount.class);
      job.setMapperClass(testMapper.class);
      job.setReducerClass(testReducer.class);
      FileInputFormat.addInputPath(job, new Path(arg0[0]));
      FileOutputFormat.setOutputPath(job,new Path(arg0[1]));
      job.submit();
      int rc = (job.waitForCompletion(true)?1:0);
      return rc;
   }
}