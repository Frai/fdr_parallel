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
        return lineRecord.getPos();

    }

    @Override
    public synchronized boolean next(LongWritable key, Text value) throws IOException {
        boolean appended, gotsomething;
        boolean retval;
        byte space[] = {' '};
        value.clear();
        gotsomething = false;
        do {
            appended = false;
            retval = lineRecord.next(lineKey, lineValue);
            if (retval) {
                if (lineValue.toString().length() > 0) {
                    byte[] rawline = lineValue.getBytes();
                    int rawlinelen = lineValue.getLength();
                    value.append(rawline, 0, rawlinelen);
                    value.append(space, 0, 1);
                    appended = true;
                }
                gotsomething = true;
            }
        } while (appended);

        //System.out.println("ParagraphRecordReader::next() returns "+gotsomething+" after setting value to: ["+value.toString()+"]");
        return gotsomething;
    }

    @Override
    public long getPos() throws IOException {
        return lineRecord.getPos();
    }
}