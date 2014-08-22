package packFDR;

import java.util.*;
import java.io.IOException;

import org.apache.hadoop.mapred.JobConf;
import org.apache.hadoop.mapred.Partitioner;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.fs.*;

public class PartitionerFDR implements Partitioner<Text, Text> {

    private String key = new String();
    private String value = new String();

    @Override
    public int getPartition(Text key, Text value, int numReduceTasks) {
        if(numReduceTasks == 0) {
            return 0;
        }

        // int sum = 0;
        // for(int i = 0; i < key.getLength(); i++) {
        //     if(key.charAt(i) == '1') {
        //         sum++;
        //     }
        // }

        // return sum % numReduceTasks;
        int hash = key.hashCode();
        if(hash < 0) {
            return Math.abs(hash) % numReduceTasks;
        } else {
            return hash % numReduceTasks;    
        }
    }

    @Override
    public void configure(JobConf arg0) {

    }
}