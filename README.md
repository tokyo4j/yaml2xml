## Usage

    yaml2xml < rc.yaml

## Example:

rc.yaml:

```yaml
core:
  xwaylandPersistence: yes

placement:
  policy: cascade

theme:
  dropShadows: yes
  font:
    - place: ActiveWindow
      weight: normal
    - place: InactiveWindow
      weight: normal

windowSwitcher:
  show: yes
  fields:
    field:
      - content: type
        width: 15%
      - content: title
        width: 85%

regions:
  region:
    - { name: top-left, x: 0%, y: 0%, width: 50%, height: 50% }
    - { name: top-right, x: 50%, y: 0%, width: 50%, height: 50% }

keyboard:
  keybind:
    - key: A-F4
      action:
        - name: Close
    - key: W-s
      action:
        - name: Execute
          command: Alacritty
    - key: W-a
      action:
        - name: Execute
          command: fuzzel

mouse:
  default:
  context:
    - name: Frame
      mousebind:
        - button: W-Left
          event: Press # "action" is renamed to "event"
          action:
            - name: Raise
            - name: Move
        - button: W-Right
          event: Drag # "action" is renamed to "event"
          action:
            - name: Resize
        - button: A-Left
          event: Press # "action" is renamed to "event"
          # None
        - button: A-Left
          event: Press # "action" is renamed to "event"
          # None
        - button: A-Right
          event: Press # "action" is renamed to "event"
          # None
        - button: A-Right
          event: Press # "action" is renamed to "event"
          # None

libinput:
  device:
    - category: touchpad
      naturalScroll: yes
      tapAndDrag: no
```

rc.xml (formatted):

```xml
<?xml version="1.0"?>
<labwc_config>
  <core>
    <xwaylandPersistence>yes</xwaylandPersistence>
  </core>
  <placement>
    <policy>cascade</policy>
  </placement>
  <theme>
    <dropShadows>yes</dropShadows>
    <font>
      <place>ActiveWindow</place>
      <weight>normal</weight>
    </font>
    <font>
      <place>InactiveWindow</place>
      <weight>normal</weight>
    </font>
  </theme>
  <windowSwitcher>
    <show>yes</show>
    <fields>
      <field>
        <content>type</content>
        <width>15%</width>
      </field>
      <field>
        <content>title</content>
        <width>85%</width>
      </field>
    </fields>
  </windowSwitcher>
  <regions>
    <region>
      <name>top-left</name>
      <x>0%</x>
      <y>0%</y>
      <width>50%</width>
      <height>50%</height>
    </region>
    <region>
      <name>top-right</name>
      <x>50%</x>
      <y>0%</y>
      <width>50%</width>
      <height>50%</height>
    </region>
  </regions>
  <keyboard>
    <keybind>
      <key>A-F4</key>
      <action>
        <name>Close</name>
      </action>
    </keybind>
    <keybind>
      <key>W-s</key>
      <action>
        <name>Execute</name>
        <command>Alacritty</command>
      </action>
    </keybind>
    <keybind>
      <key>W-a</key>
      <action>
        <name>Execute</name>
        <command>fuzzel</command>
      </action>
    </keybind>
  </keyboard>
  <mouse>
    <default></default>
    <context>
      <name>Frame</name>
      <mousebind>
        <button>W-Left</button>
        <event>Press</event>
        <action>
          <name>Raise</name>
        </action>
        <action>
          <name>Move</name>
        </action>
      </mousebind>
      <mousebind>
        <button>W-Right</button>
        <event>Drag</event>
        <action>
          <name>Resize</name>
        </action>
      </mousebind>
      <mousebind>
        <button>A-Left</button>
        <event>Press</event>
      </mousebind>
      <mousebind>
        <button>A-Left</button>
        <event>Press</event>
      </mousebind>
      <mousebind>
        <button>A-Right</button>
        <event>Press</event>
      </mousebind>
      <mousebind>
        <button>A-Right</button>
        <event>Press</event>
      </mousebind>
    </context>
  </mouse>
  <libinput>
    <device>
      <category>touchpad</category>
      <naturalScroll>yes</naturalScroll>
      <tapAndDrag>no</tapAndDrag>
    </device>
  </libinput>
</labwc_config>
```
