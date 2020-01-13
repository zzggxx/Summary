/**
 * File Name:Des.java
 * Copyright (c) 2014, doubibi All Rights Reserved.
 */
package com.doubibi.framework.util;

/**
 * Description:Des工具类 <br/>
 *
 * @author salman
 * @version 0.0.1
 * @date 2014年3月21日 下午8:13:05
 * @since JDK 1.6
 *
 * History:2014年3月21日 salman 初始创建
 */

import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;

import javax.crypto.Cipher;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

public class DesUtil {
    private final static Logger logger = Logger.getLogger(DesUtil.class);
    public final static String key = "8Z(%^2*)";
    private final static byte[] ivParameterSpec = { 8, 2, 3, 9, 5, 6, 7, 4 };

    /**
     * encryptDES:(DES加密算法). <br/>
     *
     * @author salman
     * @param encryptString     待加密字符串
     * @param encryptKeys        密钥
     * @return
     * @throws Exception
     * @return String
     */
    public static String encryptDES(String encryptString, String... encryptKeys) throws RuntimeException {
        try {
            if(StringUtils.isBlank(encryptString)){
                return null;
            }

            String encryptKey = key;
            if(encryptKeys!=null&&encryptKeys.length>0){
                encryptKey = encryptKeys[0];
            }

            IvParameterSpec zeroIv = new IvParameterSpec(ivParameterSpec);
            SecretKeySpec key = new SecretKeySpec(encryptKey.getBytes(), "DES");
            Cipher cipher = Cipher.getInstance("DES/CBC/PKCS5Padding");
            cipher.init(Cipher.ENCRYPT_MODE, key, zeroIv);
            byte[] encryptedData = cipher.doFinal(encryptString.getBytes());
            return Base64.encode(encryptedData);
        } catch (Exception ex) {
            throw new RuntimeException(ex);
        }

    }

    /**
     * decryptDES:(DES解密算法). <br/>
     *
     * @author salman
     * @param decryptString     待揭秘字符串
     * @param decryptKeys        密钥
     * @return
     * @throws Exception
     * @return String
     */
    public static String decryptDES(String decryptString, String... decryptKeys) throws RuntimeException {
        try {
            if(decryptString == null){
                return null;
            }

            String decryptKey = key;
            if(decryptKeys!=null&&decryptKeys.length>0){
                decryptKey = decryptKeys[0];
            }
            byte[] byteDecryptData = Base64.decode(decryptString);
            IvParameterSpec zeroIv = new IvParameterSpec(ivParameterSpec);
            SecretKeySpec key = new SecretKeySpec(decryptKey.getBytes(), "DES");

            // Cipher对象实际完成解密操作
            Cipher cipher = Cipher.getInstance("DES/CBC/PKCS5Padding");
            cipher.init(Cipher.DECRYPT_MODE, key, zeroIv);
            byte decryptedData[] = cipher.doFinal(byteDecryptData);

            return new String(decryptedData);
        } catch (Exception ex) {
            throw new RuntimeException(ex);
        }

    }

    public static void main(String[] args) {
        try {
            String plaintext = "resultStatus={9000};memo={};result={{\"alipay_trade_app_pay_response\":{\"code\":\"10000\",\"msg\":\"Success\",\"app_id\":\"2017112300120713\",\"auth_app_id\":\"2017112300120713\",\"charset\":\"utf-8\",\"timestamp\":\"2018-01-04 10:34:18\",\"total_amount\":\"1.00\",\"trade_no\":\"2018010421001004760543161425\",\"seller_id\":\"2088821847261142\",\"out_trade_no\":\"2018010410340700688495781\"},\"sign\":\"FL1NJkCsO4zasebqKZZm/3ZLrEF5K8cdO5fHIo683ik8iJuWGp+izJ9fNi8vm6lqQ6IEW0WiOx5pxEls6lZGJtqhYsFDqn38x8rUNgKNZFJ7qHIhEVo+dTef62Ay1fvbPMEfX2sZFHJSm0dZo/TgCW9VLKTckk+YOLqyyWvhXrABGvgZ3i0lyp1vl2iF/vwL5WKd8DKGc6E0VrG1HRhOOVUy14T4qmGwqEL9kzHdiuFselit5hbEqaSQXJ5h3OQokecq7ROI7jMvSIir93jiUlk9oxwWl+39ac9N36CiJLxyi3sUDZte/DGaIap82rNL+5roAf4/77zDzKGqEGR2Hg==\",\"sign_type\":\"RSA2\"}}";
            String ciphertext = DesUtil.encryptDES(plaintext, key);
            System.out.println("明文：" + plaintext);
            System.out.println("密文：" + ciphertext);
            System.out.println("解密后：" + DesUtil.decryptDES(ciphertext, key));
        } catch (Exception e) {
            logger.error(e);
        }
    }
}