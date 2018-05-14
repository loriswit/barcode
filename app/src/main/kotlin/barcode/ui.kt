// Created by Loris Witschard on 4/21/2018.
package barcode

import org.w3c.dom.HTMLAnchorElement
import org.w3c.dom.HTMLElement
import org.w3c.dom.HTMLInputElement
import org.w3c.dom.asList
import kotlin.browser.document

object UI
{
    init
    {
        for(node in document.querySelectorAll("#main-row button").asList())
        {
            val button = node as HTMLElement
            val row = button.id.removeSuffix("-button")
            document.getElementById(button.id)?.addEventListener("click", { displayButtonRow(row) })
        }
        
        listenAction("move-forward-button", "move-units", Action.MOVE_FORWARD)
        listenAction("move-backward-button", "move-units", Action.MOVE_BACKWARD)
        listenAction("rotate-left-button", "rotate-degrees", Action.ROTATE_LEFT)
        listenAction("rotate-right-button", "rotate-degrees", Action.ROTATE_RIGHT)
        listenAction("led-on-button", "led-number", Action.LED_ON)
        listenAction("led-off-button", "led-number", Action.LED_OFF)
        listenAction("wait-wait-button", "wait-time", Action.WAIT)
        
        val saveLink = document.getElementById("save") as HTMLAnchorElement
        saveLink.addEventListener("click", { saveLink.href = Canvas.dataURL() })
        
        hideButtonRows()
        Canvas.update()
    }
    
    private fun listenAction(buttonID: String, valueID: String, action: Action)
    {
        val button = document.getElementById(buttonID)
        val value = document.getElementById(valueID) as HTMLInputElement
        
        button?.addEventListener("click", { registerPacket(action, value.value.toInt()) })
    }
    
    private fun registerPacket(action: Action, value: Int)
    {
        Barcode.addPacket(Packet(action, value))
        
        hideButtonRows()
        Canvas.clear()
        Canvas.update()
        
        document.getElementById("packets")?.innerHTML = Barcode.toString()
    }
    
    private fun hideButtonRows()
    {
        for(row in document.getElementsByClassName("button-row").asList())
            row.classList.add("hidden")
        
        for(node in document.querySelectorAll("#main-row button").asList())
        {
            val button = node as HTMLElement
            button.classList.remove("selected")
        }
    }
    
    private fun displayButtonRow(rowID: String)
    {
        hideButtonRows()
        document.getElementById("$rowID-row")?.classList?.remove("hidden")
        document.getElementById("$rowID-button")?.classList?.add("selected")
    }
}
