package packFDR;

import java.util.*;
import java.io.IOException;
import java.lang.InterruptedException;

import org.apache.hadoop.mapred.*;
import org.apache.hadoop.mapred.RecordReader;

import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.util.LineReader;
import org.apache.hadoop.fs.*;
import org.apache.hadoop.conf.Configuration;

public class RecordReaderFDR implements RecordReader<LongWritable, Text> {

    private LineRecordReader lineRecord;
    private LongWritable lineKey;
    private Text lineValue;

    public RecordReaderFDR(JobConf conf, FileSplit split) throws IOException {
        lineRecord = new LineRecordReader(conf, split);
        lineKey = lineRecord.createKey();
        lineValue = lineRecord.createValue();
    }

    @Override
    public void close() throws IOException {
        lineRecord.close();
    }

    @Override
    public LongWritable createKey() {
        return new LongWritable();
    }

    @Override
    public Text createValue() {
        return new Text("");
    }

    @Override
    public float getProgress() throws IOException {
        return lineRecord.getProgress();
    }

    @Override
    public synchronized boolean next(LongWritable key, Text value) throws IOException {
        boolean appended, gotsomething;
        boolean retval;
        byte space[] = {'\n'};
        value.clear();
        gotsomething = false;

        do {
            appended = false;
            retval = lineRecord.next(lineKey, lineValue);
            if(retval) {
                if(lineValue.toString().length() > 0) {
                    byte[] rawline = lineValue.getBytes();
                    int rawlinelen = lineValue.getLength();
                    value.append(rawline, 0, rawlinelen);
                    value.append(space, 0, 1);
                    appended = true;
                }
                gotsomething = true;

                if(getProgress() == 1.0)
                    return gotsomething;
            }
        } while(appended);

        return gotsomething;
    }

    @Override
    public long getPos() throws IOException {
        return lineRecord.getPos();
    }
}