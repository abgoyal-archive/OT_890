/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

// Copyright 2008 the V8 project authors. All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//     * Neither the name of Google Inc. nor the names of its
//       contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef V8_D8_DEBUG_H_
#define V8_D8_DEBUG_H_


#include "d8.h"
#include "debug.h"


namespace v8 {


void HandleDebugEvent(DebugEvent event,
                      Handle<Object> exec_state,
                      Handle<Object> event_data,
                      Handle<Value> data);

// Start the remove debugger connecting to a V8 debugger agent on the specified
// port.
void RunRemoteDebugger(int port);

// Forward declerations.
class RemoteDebuggerEvent;
class ReceiverThread;


// Remote debugging class.
class RemoteDebugger {
 public:
  explicit RemoteDebugger(int port)
      : port_(port),
        event_access_(i::OS::CreateMutex()),
        event_available_(i::OS::CreateSemaphore(0)),
        head_(NULL), tail_(NULL) {}
  void Run();

  // Handle events from the subordinate threads.
  void MessageReceived(i::SmartPointer<char> message);
  void KeyboardCommand(i::SmartPointer<char> command);
  void ConnectionClosed();

 private:
  // Add new debugger event to the list.
  void AddEvent(RemoteDebuggerEvent* event);
  // Read next debugger event from the list.
  RemoteDebuggerEvent* GetEvent();

  // Handle a message from the debugged V8.
  void HandleMessageReceived(char* message);
  // Handle a keyboard command.
  void HandleKeyboardCommand(char* command);

  // Get connection to agent in debugged V8.
  i::Socket* conn() { return conn_; }

  int port_;  // Port used to connect to debugger V8.
  i::Socket* conn_;  // Connection to debugger agent in debugged V8.

  // Linked list of events from debugged V8 and from keyboard input. Access to
  // the list is guarded by a mutex and a semaphore signals new items in the
  // list.
  i::Mutex* event_access_;
  i::Semaphore* event_available_;
  RemoteDebuggerEvent* head_;
  RemoteDebuggerEvent* tail_;

  friend class ReceiverThread;
};


// Thread reading from debugged V8 instance.
class ReceiverThread: public i::Thread {
 public:
  explicit ReceiverThread(RemoteDebugger* remote_debugger)
      : remote_debugger_(remote_debugger) {}
  ~ReceiverThread() {}

  void Run();

 private:
  RemoteDebugger* remote_debugger_;
};


// Thread reading keyboard input.
class KeyboardThread: public i::Thread {
 public:
  explicit KeyboardThread(RemoteDebugger* remote_debugger)
      : remote_debugger_(remote_debugger) {}
  ~KeyboardThread() {}

  void Run();

 private:
  RemoteDebugger* remote_debugger_;
};


// Events processed by the main deubgger thread.
class RemoteDebuggerEvent {
 public:
  RemoteDebuggerEvent(int type, i::SmartPointer<char> data)
      : type_(type), data_(data), next_(NULL) {
    ASSERT(type == kMessage || type == kKeyboard || type == kDisconnect);
  }

  static const int kMessage = 1;
  static const int kKeyboard = 2;
  static const int kDisconnect = 3;

  int type() { return type_; }
  char* data() { return *data_; }

 private:
  void set_next(RemoteDebuggerEvent* event) { next_ = event; }
  RemoteDebuggerEvent* next() { return next_; }

  int type_;
  i::SmartPointer<char> data_;
  RemoteDebuggerEvent* next_;

  friend class RemoteDebugger;
};


}  // namespace v8


#endif  // V8_D8_DEBUG_H_