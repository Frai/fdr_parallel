package packFDR;

import java.util.*;
import java.io.IOException;

import org.apache.hadoop.mapred.*;
import org.apache.hadoop.mapred.JobConf;
import org.apache.hadoop.mapred.FileSplit;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.fs.*;

public class InputFormatFDR extends FileInputFormat {
    @Override
    public RecordReader<LongWritable, Text> getRecordReader(InputSplit split, JobConf conf,
            Reporter reporter) throws IOException {
        reporter.setStatus(split.toString());
        return new RecordReaderFDR(conf, (FileSplit) split);
    }
}