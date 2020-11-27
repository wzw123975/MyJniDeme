package com.wzw.myjnideme;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.ContextCompat;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.wzw.jnilibrary.Encryptor;
import com.wzw.jnilibrary.JniArrayOperation;
import com.wzw.jnilibrary.JniBitmapDemo;
import com.wzw.jnilibrary.JniFileOperation;
import com.wzw.jnilibrary.JniListDirAllFiles;
import com.wzw.jnilibrary.Utils;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
//        visit();
//        array();
//        encryptor();
//        jniFileOperation();
//        listDirAllFile();
        jniBitmap();
    }

    private void visit() {
        Utils mUtils = new Utils();
        mUtils.accessField();
        mUtils.accessStaticField();
        mUtils.accessMethod();
        mUtils.accessStaticMethod("1--0");
        Utils utils = mUtils.accessConstructor();
        utils.showText = "123";
        Toast.makeText(this, Utils.stringFromJNI() + "    " + mUtils.showText + "    " + utils.showText, Toast.LENGTH_SHORT).show();
    }

    private void array() {
        JniArrayOperation jniArrayOperation = new JniArrayOperation();
        jniArrayOperation.test();
    }

    private void encryptor() {
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                requestPermissions(new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, 0x1024);
                return;
            }
        }
        Encryptor encryptor = new Encryptor();
        encryptor.test();
    }

    private void jniFileOperation() {

        new Thread(new Runnable() {
            @Override
            public void run() {
                long time = System.currentTimeMillis();
                JniFileOperation jniFileOperation = new JniFileOperation();
                jniFileOperation.test();
                Log.i("ii--", "=========0000======" + (System.currentTimeMillis() - time));
                try {
                    time = System.currentTimeMillis();
                    splitFile("/sdcard/NDKDemo/a.zip", 5);
                    Log.i("ii--", "=========11111======" + (System.currentTimeMillis() - time));
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }).start();

    }

    private void jniBitmap() {
        JniBitmapDemo jniBitmapDemo = new JniBitmapDemo();
        jniBitmapDemo.test((ImageView)findViewById(R.id.image_view));
    }


    private void listDirAllFile() {
        new JniListDirAllFiles().test();
        Toast.makeText(this, "任务完成，检查看日志输出", Toast.LENGTH_SHORT).show();
    }


    ////////////////////////
    public static void splitFile(String filePath, int fileCount) throws IOException {
        FileInputStream fis = new FileInputStream(filePath);
        FileChannel inputChannel = fis.getChannel();
        final long fileSize = inputChannel.size();
        long average = fileSize / fileCount;//平均值
        long bufferSize = 1; //缓存块大小，自行调整
        ByteBuffer byteBuffer = ByteBuffer.allocate(Integer.valueOf(bufferSize + "")); // 申请一个缓存区
        long startPosition = 0; //子文件开始位置
        long endPosition = average < bufferSize ? 0 : average - bufferSize;//子文件结束位置
        for (int i = 0; i < fileCount; i++) {
            if (i + 1 != fileCount) {
                int read = inputChannel.read(byteBuffer, endPosition);// 读取数据
                readW:
                while (read != -1) {
                    byteBuffer.flip();//切换读模式
                    byte[] array = byteBuffer.array();
                    for (int j = 0; j < array.length; j++) {
                        byte b = array[j];
                        if (b == 10 || b == 13) { //判断\n\r
                            endPosition += j;
                            break readW;
                        }
                    }
                    endPosition += bufferSize;
                    byteBuffer.clear(); //重置缓存块指针
                    read = inputChannel.read(byteBuffer, endPosition);
                }
            } else {
                endPosition = fileSize; //最后一个文件直接指向文件末尾
            }

            FileOutputStream fos = new FileOutputStream(filePath + (i + 1));
            FileChannel outputChannel = fos.getChannel();
            inputChannel.transferTo(startPosition, endPosition - startPosition, outputChannel);//通道传输文件数据
            outputChannel.close();
            fos.close();
            startPosition = endPosition + 1;
            endPosition += average;
        }
        inputChannel.close();
        fis.close();

    }


}