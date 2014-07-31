package fdr;

import java.util.*;
import java.io.IOException;

import org.apache.hadoop.mapred.*;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.fs.*;

public class CustomFileInputFormat extends FileInputFormat<LongWritable,Text>{

    public RecordReader<LongWritable, Text> createRecordReader(
            InputSplit split, TaskAttemptContext context)
                throws IOException, InterruptedException {
        return new CustomRecordReader();
    }

    @Override
    public RecordReader<LongWritable, Text> getRecordReader(InputSplit split, JobConf job,
                                                            Reporter reporter) throws IOException {
        return new CustomRecordReader();
    }
}