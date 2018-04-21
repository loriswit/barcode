// Created by Loris Witschard on 4/21/2018.
package barcode

import org.w3c.dom.CanvasRenderingContext2D
import org.w3c.dom.HTMLCanvasElement
import org.w3c.dom.get
import kotlin.browser.document
import kotlin.browser.window

object Canvas
{
    private const val barWidth = 10.0
    private const val barHeight = 300.0
    
    private val canvas = document.getElementsByTagName("canvas")[0] as HTMLCanvasElement
    private val scene = canvas.getContext("2d") as CanvasRenderingContext2D
    
    init
    {
        canvas.width = window.innerWidth
        canvas.height = barHeight.toInt()
    }
    
    fun update()
    {
        for((index, bit) in Barcode.serialize().withIndex())
        {
            scene.fillStyle = if(bit) "black" else "white"
            scene.fillRect(index * barWidth, 0.0, barWidth, barHeight)
        }
    }
    
    fun clear()
    {
        scene.clearRect(0.0, 0.0, canvas.width.toDouble(), canvas.height.toDouble())
    }
    
    fun dataURL(): String
    {
        return canvas.toDataURL()
    }
}
