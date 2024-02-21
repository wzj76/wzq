package cn.udesk.model;

import cn.udesk.UdeskUtil;
import udesk.core.utils.UdeskUtils;

/**
 * Created by user on 2016/12/27.
 */

public class SDKIMSetting {

    private Object code;
    private Object message;

    //是否配置了引导页
    private Object enable_im_group;
    //会话是否关闭
    private Object in_session;
    //是否在工作时间
    private Object is_worktime;
    private Object has_robot;
    private Object enable_robot;
    private Object enable_sdk_robot;
    private Object enable_agent;
    private Object enable_web_im_feedback;
    private Object no_reply_hint;
    private Object robot;
    private Object investigation_when_leave;
    private Object enable_im_survey;
    //'msg', 'form'
    private Object leave_message_type;
    private Object vcall;
    private Object vc_app_id;
    private Object sdk_vcall;

    private Object agora_app_id;
    private Object server_url;
    private Object vcall_token_url;
    private Object im_survey_show_type;
    private Object leave_message_guide;
    private Object show_robot_times;
    private Object  robot_name;

//    "vcall":true,
//            "vc_app_id":"fbcc5d3df6dfa25c418910a3611020eb",
//            "sdk_vcall":false


    public String getLeave_message_type() {

        return UdeskUtils.objectToString(leave_message_type);
    }

    public void setLeave_message_type(Object leave_message_type) {
        this.leave_message_type = leave_message_type;
    }

    public boolean isInvestigation_when_leave() {
        if (investigation_when_leave instanceof Boolean) {
            return (boolean) investigation_when_leave;
        }
        return false;
    }

    public void setInvestigation_when_leave(Object investigation_when_leave) {
        this.investigation_when_leave = investigation_when_leave;
    }

    public Object getCode() {
        return code;
    }

    public void setCode(Object code) {
        this.code = code;
    }

    public Object getMessage() {
        return message;
    }

    public void setMessage(Object message) {
        this.message = message;
    }

    public boolean getEnable_im_group() {
        if (enable_im_group instanceof Boolean) {
            return (boolean) enable_im_group;
        }
        return false;
    }

    public void setEnable_im_group(Object enable_im_group) {
        this.enable_im_group = enable_im_group;
    }

    public boolean getIn_session() {
        if (in_session instanceof Boolean) {
            return (boolean) in_session;
        }
        return false;

    }

    public void setIn_session(Object in_session) {
        this.in_session = in_session;
    }

    public boolean getIs_worktime() {
        if (is_worktime instanceof Boolean) {
            return (boolean) is_worktime;
        }
        return false;
    }

    public void setIs_worktime(Object is_worktime) {
        this.is_worktime = is_worktime;
    }

    public boolean getHas_robot() {
        if (has_robot instanceof Boolean) {
            return (boolean) has_robot;
        }
        return false;

    }

    public void setHas_robot(Object has_robot) {
        this.has_robot = has_robot;
    }

    public boolean getEnable_robot() {
        if (enable_robot instanceof Boolean) {
            return (boolean) enable_robot;
        }
        return false;
    }

    public void setEnable_robot(Object enable_robot) {
        this.enable_robot = enable_robot;
    }

    public boolean getEnable_sdk_robot() {
        if (enable_sdk_robot instanceof Boolean) {
            return (boolean) enable_sdk_robot;
        }
        return false;
    }

    public void setEnable_sdk_robot(Object enable_sdk_robot) {
        this.enable_sdk_robot = enable_sdk_robot;
    }

    public String getEnable_agent() {
        if (enable_agent instanceof Boolean) {
            boolean transfer = (boolean) enable_agent;
            if (transfer) {
                return "true";
            } else {
                return "false";
            }
        }
        return "false";
    }

    public Boolean getVcall() {
        if (vcall instanceof Boolean) {
            return (boolean) vcall;
        }
        return false;
    }

    public void setVcall(Object vcall) {
        this.vcall = vcall;
    }

    public String getVc_app_id() {
        if (vc_app_id instanceof String) {
            return (String) vc_app_id;
        }
        return "";
    }

    public void setVc_app_id(Object vc_app_id) {
        this.vc_app_id = vc_app_id;
    }

    public Boolean getSdk_vcall() {
        if (sdk_vcall instanceof Boolean) {
            return (boolean) sdk_vcall;
        }
        return false;
    }

    public void setSdk_vcall(Object sdk_vcall) {
        this.sdk_vcall = sdk_vcall;
    }

    public String getAgora_app_id() {


        return UdeskUtils.objectToString(agora_app_id);
    }

    public void setAgora_app_id(Object agora_app_id) {
        this.agora_app_id = agora_app_id;
    }

    public String getServer_url() {
        return UdeskUtils.objectToString(server_url);
    }

    public void setServer_url(Object server_url) {
        this.server_url = server_url;
    }

    public void setEnable_agent(Object enable_agent) {
        this.enable_agent = enable_agent;
    }

    public boolean getEnable_web_im_feedback() {
        if (enable_web_im_feedback instanceof Boolean) {
            return (boolean) enable_web_im_feedback;
        }
        return false;
    }

    public void setEnable_web_im_feedback(Object enable_web_im_feedback) {
        this.enable_web_im_feedback = enable_web_im_feedback;
    }

    public boolean getEnable_im_survey() {
        if (enable_im_survey instanceof Boolean) {
            return (boolean) enable_im_survey;
        }
        return false;
    }

    public void setEnable_im_survey(Object enable_im_survey) {
        this.enable_im_survey = enable_im_survey;
    }

    public String getNo_reply_hint() {
        return UdeskUtils.objectToString(no_reply_hint);
    }

    public void setNo_reply_hint(Object no_reply_hint) {
        this.no_reply_hint = no_reply_hint;
    }

    public String getRobot() {

        return UdeskUtils.objectToString(robot);
    }

    public void setRobot(Object robot) {
        this.robot = robot;
    }

    public String getVcall_token_url() {
        return UdeskUtils.objectToString(vcall_token_url);
    }

    public void setVcall_token_url(Object vcall_token_url) {
        this.vcall_token_url = vcall_token_url;
    }

    public String getIm_survey_show_type() {
        return UdeskUtils.objectToString(im_survey_show_type);
    }

    public void setIm_survey_show_type(Object im_survey_show_type) {
        this.im_survey_show_type = im_survey_show_type;
    }

    public String getLeave_message_guide() {
        return UdeskUtils.objectToString(leave_message_guide);
    }

    public void setLeave_message_guide(Object leave_message_guide) {
        this.leave_message_guide = leave_message_guide;
    }

    public Object getShow_robot_times() {
        return show_robot_times;
    }

    public void setShow_robot_times(Object show_robot_times) {
        this.show_robot_times = show_robot_times;
    }

    public Object getRobot_name() {
        return robot_name;
    }

    public void setRobot_name(Object robot_name) {
        this.robot_name = robot_name;
    }
}
