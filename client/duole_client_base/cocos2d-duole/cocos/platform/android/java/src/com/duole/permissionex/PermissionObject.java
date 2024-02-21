package com.duole.permissionex;

public class PermissionObject {

    public String name = "";
    public String description = "";
    public boolean required = false;
    public boolean granted = false;

    public static PermissionObject build(String name, String description, boolean required) {
        return new PermissionObject(name, description, required);
    }

    public PermissionObject(String name, String description, boolean required) {
        this.name = name;
        this.description = description;
        this.required = required;
    }

    public String toString() {
        return "[PERMISSION"
                + " name=" + name
                + ", description=" + description
                + ", required=" + required
                + ", granted=" + granted
                + "]";
    }
}
